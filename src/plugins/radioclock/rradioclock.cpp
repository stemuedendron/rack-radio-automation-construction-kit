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

#include "rradioclock.h"
#include "rcolordialog.h"
#include <QtGui>


//todo: wenn gradbutton toggeln kann, im paintevent status auswerten !!!
//timer core coreapp nutzen
//showdate ?
//show secondsleft ?

RRadioClock::RRadioClock(QWidget *parent) : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(200);

    setMinimumSize(QSize(130,130));
    pushed = false;

}

void RRadioClock::mouseReleaseEvent(QMouseEvent *ev)
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
        }
    }
}

void RRadioClock::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //als property setzen
    //QColor color("#244BB0");
    QColor color(Qt::white);

    int side = qMin(width(), height());
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    painter.setBrush(color);
    painter.setPen(color);
    painter.setFont(QFont("Sans Serif", 18, QFont::Bold));
    QTime time = QTime::currentTime();
    painter.drawText(-100,-100,200,200, Qt::AlignCenter, time.toString("hh:mm:ss"));
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 91, 0);
        painter.rotate(30.0);
    }

    int j,k;
    if (time.second() == 0) j=60;
    else j=time.second();

    switch (pushed) {
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
