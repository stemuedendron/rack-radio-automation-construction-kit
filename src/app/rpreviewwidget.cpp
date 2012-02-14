/*
    Copyright (C) 2012, Steffen Müller and the r.a.c.k. team.
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

#include "rpreviewwidget.h"

#include <QtGui>

RPreviewWidget::RPreviewWidget(QWidget *parent) :
    QWidget(parent)
{
    setVisible(false);
//    qApp->installEventFilter(this);
}

void RPreviewWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool RPreviewWidget::eventFilter(QObject *, QEvent *)
{
//    if (obj->inherits("QLineEdit") || obj->inherits("QTextEdit")) {
//        if (event->type() == QEvent::FocusIn) {
//             QWidget *w = qobject_cast<QWidget *>(obj);
//             this->setKeyReceiver(w);

//             this->move(x(), parentWidget()->height());
//             this->setVisible(true);
//             startTimer(25);

//        }
//        if (event->type() == QEvent::FocusOut) {
//           // this->setVisible(false);
//        }
//    } else if (obj == parent()) {
//        if (event->type() == QEvent::Resize) {
//            move((parentWidget()->width() - width()) / 2,  parentWidget()->height() - height());
//        }
//    }
//    return QWidget::eventFilter(obj, event);
}

void RPreviewWidget::timerEvent(QTimerEvent *)
{
//    if (y() <= (parentWidget()->height() - height())) {
//        killTimer(event->timerId());
//        move(x(), parentWidget()->height() - height());
//    } else {
//        move(x(), y() - 30 );
//    }
}
