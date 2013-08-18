/*
    Copyright (C) 2011, Steffen Müller.
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
#include "rradioclock.h"
#include "rcolordialog.h"
#include <QtWidgets>


//todo: wenn gradbutton toggeln kann, im paintevent status auswerten !!!
//timer core coreapp nutzen
//showdate ?
//show secondsleft ?

//neue art scalierung checken!! graphics view framework????

RRadioClock::RRadioClock(ICore *api, QWidget *parent)
    : QWidget(parent),
      m_core(api),
      m_pushed(false)
{
    setMinimumSize(QSize(130,130));

    //connect to api
    QObject::connect(m_core, SIGNAL(dateStrChanged(QString)), this, SLOT(setDate(QString)));
    QObject::connect(m_core, SIGNAL(timeStrChanged(QString)), this, SLOT(setTime(QString)));
    QObject::connect(this, SIGNAL(sayHello(QString)), m_core, SLOT(getHello(QString)));
}

void RRadioClock::setDate(const QString &str)
{
    m_date = str;
    update();
}

void RRadioClock::setTime(const QString &str)
{
    m_time = str;
    update();
}

void RRadioClock::mouseReleaseEvent(QMouseEvent *ev)
{

    if (QRect(2,2,width()-4, height()-4).contains(ev->x(),ev->y(),true))
    {
        if (ev->button() == Qt::LeftButton)
        {
            m_pushed = !m_pushed;
            update();


            emit sayHello("Hello from radio clock!");


        }
        else if (ev->button() == Qt::RightButton)
        {
            bool ok;
            QColor color = RColorDialog::getColor(this, tr("Choose a color"), &ok);
            if (ok)
                //setColorButton(color)
                ;
        }
    }
}

void RRadioClock::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //QWidget descendants needs this to use style sheets:
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);


    //als property setzen
    //QColor color("#244BB0");

    //FIXME: use stylesheet
    //QColor color(Qt::white);

    int side = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);

    //FIXME: use stylesheet properties
//    painter.setBrush(color);
//    painter.setPen(color);
//    painter.setFont(QFont("Sans Serif", 18, QFont::Bold));

    painter.setBrush(palette().foreground().color());

    QTime time = QTime::fromString(m_time);

    painter.drawText(-100,-100,200,200, Qt::AlignCenter, m_time);
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 91, 0);
        painter.rotate(30.0);
    }

    int j,k;
    if (time.second() == 0) j=60;
    else j=time.second();

    switch (m_pushed) {
    case true:
        for (k = 1; k <= 60 ; ++k) {
            painter.rotate(6.0);
            if (k>time.second()) painter.drawEllipse(QPoint(0,-82),3,3);
        }
        break;
    case false:
        for (k = 1; k <= j ; ++k) {
            painter.rotate(6.0);
            painter.drawEllipse(QPoint(0,-82),3,3);
        }
        break;
    }
}
