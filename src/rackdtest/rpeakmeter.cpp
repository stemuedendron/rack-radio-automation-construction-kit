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

#include "rpeakmeter.h"
#include "rmeterbar.h"
#include <QtWidgets>

RPeakMeter::RPeakMeter(QWidget *parent)
    : QWidget(parent),
      m_leftBar(new RMeterBar),
      m_rightBar(new RMeterBar),
      m_layout(new QGridLayout),
      m_orientation(Qt::Horizontal)
{
    setMinimumSize(QSize(130,30));
    resize(300, 30);

    setStyleSheet("background-color: black;");

    m_layout->setSpacing(2);
    m_layout->setContentsMargins(2, 2, 2, 2);
    m_layout->addWidget(m_leftBar, 0, 0, 1, 2);
    m_layout->addWidget(m_rightBar, 1, 0, 1, 2);
    setLayout(m_layout);
}

Qt::Orientation RPeakMeter::orientation()
{
    return m_orientation;
}

void RPeakMeter::setLeftLevel(const int left)
{
    m_leftBar->setLevel(left);
}

void RPeakMeter::setRightLevel(const int right)
{
    m_rightBar->setLevel(right);
}

void RPeakMeter::setOrientation(const Qt::Orientation orientation)
{
    if (orientation == m_orientation) return;
    m_orientation = orientation;
    m_leftBar->setOrientation(m_orientation);
    m_rightBar->setOrientation(m_orientation);
    m_layout->removeWidget(m_leftBar);
    m_layout->removeWidget(m_rightBar);
    if (m_orientation == Qt::Horizontal)
    {
        m_layout->addWidget(m_leftBar, 0, 0, 1, 2);
        m_layout->addWidget(m_rightBar, 1, 0, 1, 2);
        setMinimumSize(QSize(130,30));
    }
    else
    {
        m_layout->addWidget(m_leftBar, 0, 0, 2, 1);
        m_layout->addWidget(m_rightBar, 0, 1, 2, 1);
        setMinimumSize(QSize(30,130));
    }
}

void RPeakMeter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}







