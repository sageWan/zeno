#ifndef __ZENO_MAINWINDOW_H__
#define __ZENO_MAINWINDOW_H__

#include <unordered_set>
#include <QtWidgets>
#include "dock/ztabdockwidget.h"

class ZenoMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    ZenoMainWindow(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~ZenoMainWindow();
    bool inDlgEventLoop() const;
    void setInDlgEventLoop(bool bOn);

public slots:
    void openFileDialog();
    void onNewFile();
    bool openFile(QString filePath);
    bool saveFile(QString filePath);
    void saveQuit();
    void save();
    void saveAs();
    void onMaximumTriggered();
    void onSplitDock(bool);
    void onToggleDockWidget(DOCK_TYPE, bool);
    void onDockSwitched(DOCK_TYPE);
    void importGraph();
    void exportGraph();
    void onNodesSelected(const QModelIndex& subgIdx, const QModelIndexList& nodes, bool select);
    void onPrimitiveSelected(const std::unordered_set<std::string>& primids);
    void updateViewport(const QString& action = "");
    void onRunFinished();
    void onFeedBack();
    void clearErrorMark();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void init();
    void initMenu();
    void initDocks();
    void verticalLayout();
    void onlyEditorLayout();
    void adjustDockSize();
    void recordRecentFile(const QString& filePath);
    QString getOpenFileByDialog();
    QString uniqueDockObjName(DOCK_TYPE type);

    bool m_bInDlgEventloop;
};

#endif
