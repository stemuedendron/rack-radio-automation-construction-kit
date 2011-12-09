#include "mainwindow.h"
#include <QVBoxLayout>

#include <QtGui>

#include "fsgradbutton.h"
#include "fscartbutton.h"
#include "fsplaybutton.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{

    FSGradButton *gbt = new FSGradButton;
    gbt->setPixmap(QPixmap(":/images/left.png"));

    FSCartButton *cbt = new FSCartButton;
    //cbt->setTextStatus("Test");

    FSPlayButton *pbt = new FSPlayButton;
    QPixmap pixmap(":/images/play.png");
    pbt->setPixmap(pixmap);

    QSlider *sl = new QSlider(Qt::Horizontal);
    sl->setRange(0,360);
    connect(sl,SIGNAL(valueChanged(int)),cbt,SLOT(setPositionClock(int)));
    connect(sl,SIGNAL(valueChanged(int)),pbt,SLOT(setPositionClock(int)));

    QVBoxLayout *l = new QVBoxLayout;
    l->addWidget(gbt);
    l->addWidget(cbt);
    l->addWidget(pbt);
    l->addWidget(sl);
    setLayout(l);
}
