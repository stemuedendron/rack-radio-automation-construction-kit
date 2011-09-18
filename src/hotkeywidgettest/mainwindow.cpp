#include "mainwindow.h"
#include <QtGui>
#include "rbutton.h"
#include "rhotkeys.h"


/*
default styles and images are in resource file (so no style files must be shipped)
 user styles are in data location setting per config/ini

user fonts must be loaded (config):
QFontDatabase fontDB;
fontDB.addApplicationFont(":/fonts/droidSansRegular.ttf");
fontDB.addApplicationFont(":/fonts/droidSansBold.ttf");
the path must be given!!!

use unpolish polish

in enums set integer value bkBla=1 etc. and check in style bk="1"
*/



MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{


    setObjectName("rackWidget");

    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);


    QGridLayout *l = new QGridLayout;
    l->setSpacing(1);

    RHotKeys *hk = new RHotKeys;
    l->addWidget(hk);

//    QPushButton *b0 = new QPushButton;
//    b0->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
//    l->addWidget(b0);


//    RButton *b1 = new RButton(RButton::bkGrad);
//    l->addWidget(b1);

//    RButton *b2 = new RButton(RButton::bkOnAir);
//    b2->setLength(2000);
//    l->addWidget(b2);

//    RButton *b3 = new RButton(RButton::bkStack);
//    l->addWidget(b3);

//    RButton *b4 = new RButton(RButton::bkHotKey);
//    b4->setLength(2000);
//    l->addWidget(b4);

//    RButton *b5 = new RButton(RButton::bkLibrary);
//    l->addWidget(b5);

//    QSlider *sl = new QSlider;
//    sl->setOrientation(Qt::Horizontal);
//    sl->setRange(0,2000);
//    l->addWidget(sl);

//    connect(sl,SIGNAL(valueChanged(int)),b2,SLOT(setPosition(int)));
//    connect(sl,SIGNAL(valueChanged(int)),b4,SLOT(setPosition(int)));

    setLayout(l);


//    QVBoxLayout *vl = new QVBoxLayout(this);
//    vl->setMargin(0);
//    vl->setSpacing(0);
//    setLayout(vl);
//    RButton *b6 = new RButton(RButton::bkLibrary);
//    RButton *b7 = new RButton(RButton::bkLibrary);
//    QPushButton *b8 = new QPushButton;
//    QPushButton *b9 = new QPushButton;
//    vl->addWidget(b6);
//    vl->addWidget(b7);
//    vl->addWidget(b8);
//    vl->addWidget(b9);

//    QString text = QInputDialog::getText(this,"titel","Label");
//    QMessageBox::information(this, "lighter",QColor(text).lighter().name());
//    QMessageBox::information(this, "hover",QColor(text).lighter(120).name());
//    QMessageBox::information(this, "hover-lighter",QColor(text).lighter().lighter(120).name());

}
