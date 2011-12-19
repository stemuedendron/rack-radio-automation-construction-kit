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

#ifndef RDIGITALCLOCK_H
#define RDIGITALCLOCK_H

#include <QPushButton>

class RDigitalClock : public QWidget
{
    Q_OBJECT

public:

    explicit RDigitalClock(QWidget *parent = 0);

public slots:

    void setDate(const QString &);
    void setTime(const QString &);

signals:

    void sayHello(const QString &);

protected:

    void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:

    bool m_pushed;
    QString m_date;
    QString m_time;

};

#endif // RDIGITALCLOCK_H
