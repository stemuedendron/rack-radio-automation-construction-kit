#include <QtGui>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "rdesktop.h"



RMainWindow::RMainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("R.A.C.K.");
    //setPalette(QPalette(Qt::black));

    //better:
    //setStyleSheet("QWidget { background-color: black; }");

    m_desktop = new RDesktop;

    m_btSettings = new QPushButton("Settings");
    m_btSettings->setCheckable(true);
    QObject::connect(m_btSettings, SIGNAL(toggled(bool)), m_desktop, SLOT(setConfigModus(bool)));

    QPushButton *btQuit = new QPushButton("Quit");
    QObject::connect(btQuit,SIGNAL(clicked()),SLOT(close()));

    m_btSavetest = new QPushButton("Save");
    QObject::connect(m_btSavetest, SIGNAL(clicked()), m_desktop, SLOT(saveFSWidgets()));


    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addWidget(m_btSettings);
    barLayout->addWidget(btQuit);

    barLayout->addWidget(m_btSavetest);



    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_desktop);
    layout->addLayout(barLayout);
    setLayout(layout);

    startTimer(1000);


}

void RMainWindow::timerEvent(QTimerEvent *)
 {
     emit timeStrChanged(QTime::currentTime().toString(tr("hh:mm:ss")));
     emit dateStrChanged(QDate::currentDate().toString(tr("dd.MM.yy")));
 }
