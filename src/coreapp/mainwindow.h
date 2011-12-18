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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

//forward deklarationen:
class RDesktop;
class QWidget;
class QPushButton;

class RMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RMainWindow(QWidget *parent = 0);

signals:

    void timeStrChanged(QString);
    void dateStrChanged(QString);

private:
    RDesktop *m_desktop;

    QWidget *m_taskbar;
    QPushButton *m_btSettings;
    QPushButton *m_btSavetest;

protected:
     void timerEvent(QTimerEvent *);

};

#endif // MAINWINDOW_H
