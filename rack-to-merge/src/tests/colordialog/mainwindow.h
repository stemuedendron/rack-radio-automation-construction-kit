#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fsgradbutton.h"

class MainWindow : public QMainWindow
{

   Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);

private slots:

    void setColor();

private:

    FSGradButton *bt;

};

#endif // MAINWINDOW_H
