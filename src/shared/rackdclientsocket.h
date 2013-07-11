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

#ifndef RACKDCLIENTSOCKET_H
#define RACKDCLIENTSOCKET_H

#include <QTcpSocket>


class RackdClientSocket : public QTcpSocket
{

    Q_OBJECT

public:

    explicit RackdClientSocket(QObject *parent = 0);
    bool isAuth() const {return m_isAuth;}
    void setAuth(bool ok) {m_isAuth = ok;}
    quint16 clientMeterPort() const {return m_meterPort;}
    void setClientMeterPort(const quint16 &port) {m_meterPort = port;}


signals:

    void newBlock(RackdClientSocket *client, const QByteArray &block);
    void disconnectedClient(RackdClientSocket *client);


private slots:

    void disconnectedID();


public slots:

    void readyData();


private:

    quint16 m_nextBlockSize;
    bool m_isAuth;
    quint16 m_meterPort;

};

#endif // RACKDCLIENTSOCKET_H
