#include <vector>
#include <type_traits>
#include <magic_enum.hpp>
#include <iostream>

using std::cout;
using std::endl;

/* meta.h */

template <int First, int Last, typename Lambda>
inline constexpr bool static_for(Lambda const &f) {
    if constexpr (First < Last) {
        if (f(std::integral_constant<int, First>{})) {
            return true;
        } else {
            return static_for<First + 1, Last>(f);
        }
    }
    return false;
}

template <class...>
struct type_list {
};

template <class T, class ...Ts>
struct type_list<T, Ts...> {
    using head = T;
    using rest = type_list<Ts...>;
};

template <class L, unsigned int N>
struct type_list_nth {
    using type = typename type_list_nth<typename L::rest, N - 1>::type;
};

template <class L>
struct type_list_nth<L, 0> {
    using type = typename L::head;
};

template <class L, class T>
struct type_list_find {
    static constexpr int value = type_list_find<typename L::rest, T>::value + 1;
};

template <class T, class ...Ts>
struct type_list_find<type_list<T, Ts...>, T> {
    static constexpr int value = 0;
};

/* dtype.h */

enum class dtype : int {
    none, i32, f32,
};

using dtype_type_list = type_list<void, int, float>;

template <dtype dt>
struct dtype_to_type {
    using type = typename type_list_nth<dtype_type_list, int(dt)>::type;
};

template <class T>
struct type_to_dtype {
    static constexpr dtype value = magic_enum::enum_cast<dtype>(
        type_list_find<dtype_type_list, T>::value).value();
};

constexpr auto dtype_name(dtype dt) {
    return magic_enum::enum_name(dt);
}

constexpr size_t dtype_size(dtype dt) {
    size_t ret = 0;
    static_for<0, magic_enum::enum_values<dtype>().size()>([&](auto i) {
        constexpr auto t = magic_enum::enum_cast<dtype>(i).value();
        if (dt == t) {
            using T = typename dtype_to_type<t>::type;
            if constexpr (std::is_void_v<T>)
                ret = 0;
            else
                ret = sizeof(T);
            return true;
        }
        return false;
    });
    return ret;
}

/* array.h */

template <class T = void>
struct fat_ptr {
    T *base;
    size_t size;
};

struct array {
    std::vector<char> m_data;
    dtype m_type;
    size_t m_size;

    constexpr dtype type() const { return m_type; }
    void *data() const { return (void *)m_data.data(); }
    size_t size() const { return m_size; }

    void resize(size_t n) {
        m_size = n;
        m_data.resize(n * dtype_size(m_type));
    }
};

template <class T>
void do_apply(fat_ptr<T> fp) {
    printf("%s %d\n", typeid(T).name(), fp.size);
}

template <class T>
auto arr_to_fatptr(array *a) {
    return fat_ptr<T>{(T *)a->data(), a->size()};
}

void apply(array *a) {
    auto dt = a->type();
    static_for<0, magic_enum::enum_values<dtype>().size()>([&](auto i) {
        constexpr auto t = magic_enum::enum_cast<dtype>(i).value();
        if (dt == t) {
            using T = typename dtype_to_type<t>::type;
            do_apply<T>(arr_to_fatptr<T>(a));
            return true;
        }
        return false;
    });
}

/* main.cpp */

int main(void)
{
    auto a = new array;
    a->m_type = dtype::i32;
    a->resize(128);
    apply(a);
    delete a;
    return 0;
}
