#include <QtGui>
#include "fsdigitalclock.h"
#include "fscolordialog.h"

 FSDigitalClock::FSDigitalClock(QWidget *parent) : FSGradButton(parent)
 {
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(update()));
     timer->start(1000);

     pushed = false;
     setColorButton(Qt::black);

     QFontDatabase fontDB;
     fontDB.addApplicationFont(":/fonts/digital-7.ttf");
 }

 void FSDigitalClock::mouseReleaseEvent(QMouseEvent *ev)
 {
     FSGradButton::mouseReleaseEvent(ev);

     if (QRect(2,2,width()-4, height()-4).contains(ev->x(),ev->y(),true))
     {
         if (ev->button() == Qt::LeftButton)
         {
             pushed = !pushed;
             update();
         }
         else if (ev->button() == Qt::RightButton)
         {
             bool ok;
             QColor color = FSColorDialog::getColor(this, tr("Choose a color"), &ok);
             if (ok) setColorButton(color);
             update();
         }
     }
 }

 void FSDigitalClock::paintEvent(QPaintEvent *ev)
 {
     //paint gradient
     FSGradButton::paintEvent(ev);


     //als property setzen
     //QColor color("#244BB0");
     QColor color(Qt::white);

     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setPen(color);
     painter.translate(width() / 2, height() / 2);
     painter.scale(width() / 100, width() / 100);
     painter.setFont(QFont("Digital-7", 16));
     QString label;

     switch (pushed) {
     case true:
         label = QDate::currentDate().toString(tr("dd.MM.yy"));
         break;
     case false:
         label = QTime::currentTime().toString(tr("hh:mm:ss"));
         break;
     }

     painter.drawText(-50,-50,100,100, Qt::AlignCenter, label);

 }
