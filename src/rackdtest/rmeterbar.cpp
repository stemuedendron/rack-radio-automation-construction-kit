/*
    Copyright (C) 2014, Steffen Müller.
    All rights reserved.

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

#include "rmeterbar.h"
#include <QtWidgets>

RMeterBar::RMeterBar(QWidget *parent)
    : QWidget(parent),
      m_level(0),
      m_oldLevel(0),
      m_orientation(Qt::Horizontal)
{
}

void RMeterBar::setLevel(const int level)
{
    if (level > m_oldLevel)
    {
        m_level = level;
    }
    else
    {
        (m_level - 900) > 0 ? m_level -= 900 : m_level = 0;
    }
    m_oldLevel = m_level;
    update();
}

void RMeterBar::setOrientation(const Qt::Orientation orientation)
{
    m_orientation = orientation;
    update();
}

void RMeterBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    if (m_orientation == Qt::Horizontal)
    {
        int level = m_level * width() / 32768;
        painter.fillRect(0, 0, level, height(), QColor("#FF9900"));
    }
    else
    {
        int level = m_level * height() / 32768;
        painter.fillRect(0, height() - level, width(), level, QColor("#FF9900"));
    }

}
