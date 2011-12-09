#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

//forward deklarationen:
class RDesktop;
class QWidget;
class QPushButton;

class RMainWindow : public QWidget
{
    Q_OBJECT

public:
    RMainWindow(QWidget *parent = 0);

signals:

    void timeStrChanged(QString);
    void dateStrChanged(QString);

private:
    RDesktop *m_desktop;

    QWidget *m_taskbar;
    QPushButton *m_btSettings;
    QPushButton *m_btSavetest;

protected:
     void timerEvent(QTimerEvent *);

};

#endif // MAINWINDOW_H
