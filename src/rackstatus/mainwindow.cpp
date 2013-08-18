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

#include "mainwindow.h"
#include "rackdclient.h"

#include <QtWidgets>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_rackdClient(new RackdClient(this)),
      m_log(new QTextEdit),
      m_connected(false),
      m_timer(new QTimer(this))
{

    setCentralWidget(m_log);

    connect(m_rackdClient, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_rackdClient, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_rackdClient, SIGNAL(position(quint8,quint32,quint32)), this, SLOT(position(quint8,quint32,quint32)));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(timer()));
    m_timer->start(1000);

}


void MainWindow::connected()
{
    m_connected = true;
    m_rackdClient->passWord("pass");
    m_rackdClient->meterEnable(true);
    m_timer->stop();
}


void MainWindow::disconnected()
{
    m_connected = false;
    m_timer->start(1000);
}


void MainWindow::timer()
{
    m_rackdClient->connectToRackd(QHostAddress::Any, 1234);
}


void MainWindow::position(quint8 device, quint32 handle, quint32 position)
{
    m_log->append(QString::number(device) + ":" + QString::number(handle) + ":" + QString::number(position));

}
