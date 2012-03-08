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

#include "rpushbutton.h"
#include "icore.h"
#include <QtGui>

RPushButton::RPushButton(QWidget *parent) :
    QPushButton(parent),
    m_state(Rack::NormalState)
{
    setFocusPolicy(Qt::NoFocus);
}

RPushButton::RPushButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent),
    m_state(Rack::NormalState)
{
    setFocusPolicy(Qt::NoFocus);
}

void RPushButton::setState(Rack::CoreState state)
{
    m_state = state;
    style()->unpolish(this);
    style()->polish(this);
}

void RPushButton::setUnchecked()
{
    if (isCheckable()) setChecked(false);
}


