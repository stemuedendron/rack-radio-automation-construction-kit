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
    QWidget(parent),
    m_ani(new QPropertyAnimation(this, "pos", this)),
    m_in(false)
{
    setObjectName("rackPreviewWidget");
    qApp->installEventFilter(this);
    m_ani->setDuration(50);

    RPushButton *play = new RPushButton(tr("Play"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(play);

    setLayout(layout);

}

void RPreviewWidget::fade(bool in)
{
    raise();
    m_ani->stop();
    in ? m_ani->setStartValue(QPoint(x(), parentWidget()->height())) : m_ani->setStartValue(QPoint(x(), parentWidget()->height() - height() + 4));
    in ? m_ani->setEndValue(QPoint(x(), parentWidget()->height() - height() + 4)) : m_ani->setEndValue(QPoint(x(), parentWidget()->height()));
    m_ani->start();
    m_in = in;
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

            //calc x position
            int x = (parentWidget()->width() - width()) / 2;
            QToolBar *mainToolBar = parentWidget()->findChild<QToolBar *>("rackMainToolBar");
            if (mainToolBar)
            {
                if (x <= mainToolBar->width() + 1) x = mainToolBar->width() + 2;
            }

            //calc y position
            int y = 0;
            m_in ? y = parentWidget()->height() - height() + 4 : y = parentWidget()->height();

            move(x, y);
        }
    }
    return QWidget::eventFilter(obj, event);
}



