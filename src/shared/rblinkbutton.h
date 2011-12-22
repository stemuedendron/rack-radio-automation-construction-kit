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

#ifndef RBLINKBUTTON_H
#define RBLINKBUTTON_H

#include <QPushButton>
#include <QTimer>

class RBlinkButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool blinking READ blinking)

public:

    explicit RBlinkButton(const QString &text, QWidget *parent = 0);
    int blinking() const {return m_blinking;}

signals:

private slots:

    void startTimer(const bool &);
    void setBlinking();

private:

    bool m_blinking;
    QTimer *m_timer;

};

#endif // RBLINKBUTTON_H
