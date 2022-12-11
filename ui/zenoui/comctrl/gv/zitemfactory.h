#ifndef __ZITEM_FACTORY_H__
#define __ZITEM_FACTORY_H__

#include "zenoparamwidget.h"
#include "callbackdef.h"

namespace zenoui
{
    QGraphicsItem* createItemWidget(
        const QVariant& value,
        PARAM_CONTROL ctrl,
        const QString& type,
        Callback_EditFinished cbFunc,
        QGraphicsScene* scene,
        CALLBACK_SWITCH cbSwitch,
        const QVariant& controlProps
    );
}




#endif