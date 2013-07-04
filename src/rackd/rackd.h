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

#ifndef RACKD_H
#define RACKD_H

#include <QTcpServer>
#include <QDataStream>
#include "bass.h"


class Rackd : public QTcpServer
{
    Q_OBJECT

public:

    explicit Rackd(QObject *parent = 0);
    ~Rackd();
    

public slots:

    void doCleanUp();


private slots:

    //connection handling:
    void clientConnected();
    void clientDisconnected();
    void handleError(QAbstractSocket::SocketError);

    //api handling:
    void handleRequest();


private:

    struct ClientData
    {
        bool isAuth;
        QList<quint32> handleList;
    };

    void sendBlock(QTcpSocket *client, QByteArray &response);

    //tcp transmission stuff:
    quint16 m_maxConnections;
    quint16 m_nextBlockSize;

    QList<int> m_devices;

    //    QHash<QTcpSocket *, bool> m_auth;

        //test hash with struct:
        QHash<QTcpSocket *, ClientData> m_clients;

//    QHash<QTcpSocket *, QList<HSTREAM> m_clients;


};

#endif // RACKD_H
