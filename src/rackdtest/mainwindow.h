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

class RackdSocket;
class QLineEdit;
class QTextEdit;

class MainWindow : public QWidget
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

private slots:

    void connectToServer();
    void sendPass();
    void loadStream();
    void dropConnection();

    void passWordOK(bool ok);

    
private:

    RackdSocket *m_rackdSocket;
    QLineEdit *m_le;
    QTextEdit *m_log;
    
};

#endif // MAINWINDOW_H
