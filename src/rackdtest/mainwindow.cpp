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

#include "mainwindow.h"
#include "rackdclient.h"
#include <QtWidgets>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

    m_rackdClient = new RackdClient(this);
    m_le = new QLineEdit;
    QPushButton *bConn = new QPushButton("connect");
    QPushButton *bPW = new QPushButton("send password");
    QPushButton *bLS = new QPushButton("load stream");
    QPushButton *bDC = new QPushButton("drop connection");

    m_log = new QTextEdit;

    connect(bConn, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(bPW, SIGNAL(clicked()), this, SLOT(sendPass()));
    connect(bLS, SIGNAL(clicked()), this, SLOT(loadStream()));
    connect(bDC, SIGNAL(clicked()), this, SLOT(dropConnection()));


    connect(m_rackdClient, SIGNAL(passWordOK(bool)), this, SLOT(passWordOK(bool)));

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(m_le);
    l->addWidget(bConn);
    l->addWidget(bPW);
    l->addWidget(bLS);
    l->addWidget(bDC);
    l->addWidget(m_log);

    setLayout(l);

}

void MainWindow::connectToServer()
{
    m_rackdClient->connectToRackd(QHostAddress::Any, 1234);
}


void MainWindow::sendPass()
{
    m_rackdClient->passWord(m_le->text());
}


void MainWindow::loadStream()
{
    m_rackdClient->loadStream(quint8(2), m_le->text());
}


void MainWindow::dropConnection()
{
    m_rackdClient->dropConnection();
}


void MainWindow::passWordOK(bool ok)
{
    ok ? m_log->append("password ok") : m_log->append("password not ok");
}
