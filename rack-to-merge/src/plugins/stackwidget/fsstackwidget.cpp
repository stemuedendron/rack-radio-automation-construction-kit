
#include <QSlider>
#include <QGridLayout>
#include <QPushButton>



#include "fsstackwidget.h"

#include "fsgradbutton.h"
#include "fsclockbutton.h"
#include "fsstackbutton.h"
#include "fsplaybutton.h"
#include "fscartbutton.h"


FSStackWidget::FSStackWidget(QWidget *parent) : QWidget(parent)
{

            FSPlayButton *b0 = new FSPlayButton(this);

            FSPlayButton *b1 = new FSPlayButton(this);
            FSPlayButton *b2 = new FSPlayButton(this);

            FSStackButton *b3 = new FSStackButton(this);

            FSGradButton *b4 = new FSGradButton(this);
            FSGradButton *b5 = new FSGradButton(this);

            FSCartButton *b6 = new FSCartButton(this);

            b3->setColorButton(Qt::black);
            b3->setTitle("Ich Maschine");
            b3->setArtist("Blumfeld");
            b3->setComment("Die Erste Platte 1992");
            b3->setTime("3:25");
            b3->setPlayTime("13:14:06");
            b3->setID("RF-3456");

            b4->setColorButton("#244BB0");
            b5->setColorButton("#244BB0");

            QSlider *sl = new QSlider(this);
            sl->setRange(0, 360);
            sl->setOrientation(Qt::Horizontal);
            QObject::connect(sl, SIGNAL(valueChanged(int)), b0, SLOT(setPositionClock(int)));
            QObject::connect(sl, SIGNAL(valueChanged(int)), b6, SLOT(setPositionClock(int)));

            QGridLayout *l = new QGridLayout;
            l->setSpacing(0);
            l->setContentsMargins(0,0,0,0);
            l->addWidget(b0, 0, 0);
            l->addWidget(b1, 1, 0);
            l->addWidget(b2, 2, 0);
            l->addWidget(b3, 0, 1);
            l->addWidget(b4, 1, 1);
            l->addWidget(b5, 2, 1);

            l->addWidget(b6, 0, 2);

            l->addWidget(sl, 3, 1);

            setLayout(l);



            //test animation:



}



