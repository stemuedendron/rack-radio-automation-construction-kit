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

#include "icore.h"
#include "rdigitalclock.h"
#include "rcolordialog.h"
#include <QtGui>


RDigitalClock::RDigitalClock(ICore *api, QWidget *parent)
     : QWidget(parent),
       m_core(api),
       m_pushed(false)
 {

//     QFontDatabase fontDB;
//     fontDB.addApplicationFont(":/fonts/digital-7.ttf");

     //connect to api
     QObject::connect(m_core, SIGNAL(dateStrChanged(QString)), this, SLOT(setDate(QString)));
     QObject::connect(m_core, SIGNAL(timeStrChanged(QString)), this, SLOT(setTime(QString)));
     QObject::connect(this, SIGNAL(sayHello(QString)), m_core, SLOT(getHello(QString)));

 }

 void RDigitalClock::setDate(const QString &str)
 {
     m_date = str;
     update();
 }

 void RDigitalClock::setTime(const QString &str)
 {
     m_time = str;
     update();
 }

 void RDigitalClock::mouseReleaseEvent(QMouseEvent *ev)
 {

     if (QRect(2,2,width()-4, height()-4).contains(ev->x(),ev->y(),true))
     {
         if (ev->button() == Qt::LeftButton)
         {
             m_pushed = !m_pushed;
             update();




             emit sayHello("Hello from digital clock!");




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

     //QColor color(Qt::white);

     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing);

     //QWidget descendants needs this to use style sheets:
     QStyleOption opt;
     opt.init(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

     //painter.setPen(color);

     painter.translate(width() / 2, height() / 2);
     painter.scale(width() / 100, width() / 100);

     //FIXME: use stylesheet
     //painter.setFont(QFont("Digital-7", 16));

     QString label;

     switch (m_pushed) {
     case true:
         label = m_date;
         break;
     case false:
         label = m_time;
         break;
     }

     painter.drawText(-50,-50,100,100, Qt::AlignCenter, label);

 }
