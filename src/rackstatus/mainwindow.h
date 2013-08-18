/*
    Copyright (C) 2011, Steffen Müller.
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

#include <QMainWindow>


class RackdClient;
class QTextEdit;
class QTimer;


class MainWindow : public QMainWindow
{

    Q_OBJECT
    

public:

    MainWindow(QWidget *parent = 0);


private slots:

    void connected();
    void disconnected();
    void timer();
    void position(quint8 device, quint32 handle, quint32 position);


private:

    RackdClient *m_rackdClient;
    QTextEdit *m_log;
    bool m_connected;
    QTimer *m_timer;

};

#endif // MAINWINDOW_H
