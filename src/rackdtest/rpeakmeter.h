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

#ifndef RPEAKMETER_H
#define RPEAKMETER_H

#include <QWidget>

class RMeterBar;
class QGridLayout;

class RPeakMeter : public QWidget
{
    Q_OBJECT

public:

    explicit RPeakMeter(QWidget *parent = 0);
    Qt::Orientation orientation();

public slots:

    void setLeftLevel(const int left);
    void setRightLevel(const int right);
    void setOrientation(const Qt::Orientation orientation);

protected:

    void paintEvent(QPaintEvent *);

private:

    RMeterBar *m_leftBar;
    RMeterBar *m_rightBar;
    QGridLayout *m_layout;
    Qt::Orientation m_orientation;

};

#endif // RPEAKMETER_H
