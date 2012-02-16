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
#include "rpushbutton.h"

#include <QtGui>

RPreviewWidget::RPreviewWidget(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("rackPreviewWidget");
    qApp->installEventFilter(this);

    RPushButton *play = new RPushButton(tr("Play"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(play);

    setLayout(layout);

}

void RPreviewWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool RPreviewWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == parent())
    {
        if (event->type() == QEvent::Resize) {
            move((parentWidget()->width() - width()) / 2,  parentWidget()->height() - height());
        }
    }
    return QWidget::eventFilter(obj, event);
}


