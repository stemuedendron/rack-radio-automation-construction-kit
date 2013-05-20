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

#include "rblinkbutton.h"

#include <QtWidgets>

RBlinkButton::RBlinkButton(const QString &text, QWidget *parent) :
    RPushButton(text, parent),
    m_blinking(false),
    m_interval(300)
{
}

void RBlinkButton::setInterval(int interval)
{
    m_interval = interval;
    if (m_timer.isActive()) m_timer.start(m_interval, this);
}

void RBlinkButton::startBlinking()
{
    toggleBlinking(true);
    m_timer.start(m_interval, this);
}

void RBlinkButton::stopBlinking()
{
    m_timer.stop();
    toggleBlinking(false);
}

void RBlinkButton::setBlinking(bool blink)
{
    blink ? startBlinking() : stopBlinking();
}

void RBlinkButton::timerEvent(QTimerEvent *)
{
    toggleBlinking(!m_blinking);
}

void RBlinkButton::toggleBlinking(bool blink)
{
    m_blinking = blink;
    style()->unpolish(this);
    style()->polish(this);
}
