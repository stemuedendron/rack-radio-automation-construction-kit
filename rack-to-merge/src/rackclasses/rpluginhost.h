#ifndef RPLUGINHOST_H
#define RPLUGINHOST_H
#include <QWidget>
#include "fsinterface.h"

class QHBoxLayout;
class QVBoxLayout;
class FSLayoutWidget;
class FSGradButton;
class FSCartButton;
class QSignalMapper;

class RPluginHost : public QWidget
{
    Q_OBJECT

public:

    RPluginHost(QWidget *parent = 0);

signals:

    void btNewWidgetCLick(int);
    void btCloseWidgetClick();

    void pluginAdded();

    void setPluginsVisible(bool);

protected:

    //we reimplemented the resizeEvent() handler for resizising the layoutconfig widget
    void resizeEvent(QResizeEvent *ev);

public slots:

    void setLayoutVisible(bool);

    void newPlugin();

    void removePlugin(int);
    void showPlugin(int);

private:

    //widgets for settings
    //FSLayoutWidget *m_layoutWidget;
    QWidget *m_layoutWidget;


    QVBoxLayout *m_middlelayout;
    FSGradButton *m_btLeft;
    FSGradButton *m_btRight;
    FSGradButton *m_btTop;
    FSGradButton *m_btBottom;
    FSGradButton *m_btClose;

    FSCartButton *m_btMiddle;
    QSignalMapper *m_signalMapper;


    bool loadPlugin();

    QWidgetList m_wlPluginWidgets;

    FSInterface *m_fsInterface;
    QHBoxLayout *m_mainlayout;

};

#endif // RPLUGINHOST_H
