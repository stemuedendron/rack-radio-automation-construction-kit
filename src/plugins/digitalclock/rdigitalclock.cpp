/*
    Copyright (C) 2011, Steffen Müller and the r.a.c.k. team.
    All rights reserved.

    This file is part of r.a.c.k. radio automation construction kit

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Author: Steffen Müller
*/

#include "rdigitalclock.h"
#include "rcolordialog.h"
#include <QtGui>


 RDigitalClock::RDigitalClock(QWidget *parent) : QWidget(parent)
 {
     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(update()));
     timer->start(1000);

     pushed = false;

     QFontDatabase fontDB;
     fontDB.addApplicationFont(":/fonts/digital-7.ttf");
 }

 void RDigitalClock::mouseReleaseEvent(QMouseEvent *ev)
 {

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
             QColor color = RColorDialog::getColor(this, tr("Choose a color"), &ok);
             if (ok)
                 //setColorButton(color)
                 ;
             update();
         }
     }
 }

 void RDigitalClock::paintEvent(QPaintEvent *)
 {

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
