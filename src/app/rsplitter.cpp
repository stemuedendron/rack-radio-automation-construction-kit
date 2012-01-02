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

#include "rsplitter.h"
#include "icore.h"
#include <QtGui>

RSplitterHandle::RSplitterHandle(Qt::Orientation orientation, RSplitter *parent) : QSplitterHandle(orientation, parent)
{
    m_handleColor=Qt::white;
    QObject::connect(ICore::instance(), SIGNAL(enterSettingsMode()), this, SLOT(enterSettingsMode()));
    QObject::connect(ICore::instance(), SIGNAL(leaveSettingsMode()), this, SLOT(leaveSettingsMode()));
}

void RSplitterHandle::paintEvent(QPaintEvent *event)
{
    if (isEnabled())
    {
    QPainter painter(this);
    painter.fillRect(event->rect(), m_handleColor);
    }
}

void RSplitterHandle::mousePressEvent(QMouseEvent *)
{
    m_handleColor=Qt::red;
    update();
}

void RSplitterHandle::mouseReleaseEvent(QMouseEvent *)
{
    m_handleColor=Qt::white;
    update();
}

void RSplitterHandle::enterSettingsMode()
{
    setEnabled(true);
}

void RSplitterHandle::leaveSettingsMode()
{
    setDisabled(true);
}


RSplitter::RSplitter(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{
    setChildrenCollapsible(false);
    setHandleWidth(3);
}

QSplitterHandle *RSplitter::createHandle()
{
    RSplitterHandle *myhandle = new RSplitterHandle(orientation(), this);
    return myhandle;
}


