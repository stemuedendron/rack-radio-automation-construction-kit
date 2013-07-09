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
#include <QFutureWatcher>
#include "bass.h"


class RackdClientSocket;


class RStreamLoadURLData
{
public:
    RStreamLoadURLData() {}
    RackdClientSocket*client;
    quint8 device;
    QString uri;
    quint32 time;
    quint32 handle;
};


class Rackd : public QTcpServer
{
    Q_OBJECT

public:

    explicit Rackd(QObject *parent = 0);
    ~Rackd();
    

public slots:

    void doCleanUp();


protected:

    void incomingConnection(qintptr socketId);


private slots:

    //connection handling:
    void clientDisconnected(RackdClientSocket *client);
    void handleError(QAbstractSocket::SocketError);

    //protocol handling:
    void handleRequest(RackdClientSocket *client, const QByteArray &request);

    //loadStreamURL thread finished slot:
    void loadStreamFinished();


private:

    //protocol handling:
    void sendResponse(RackdClientSocket *client, const QByteArray &response);

    quint16 m_maxConnections;

    QList<RackdClientSocket *> m_clients;

    QList<quint8> m_devices;


//    struct RStreamData
//    {
//        HSTREAM handle;
//        RackdClientSocket *client;
//        quint8 device;
//    };
//    QList<RStreamData> m_streams;


    //loadStreamURL thread stuff:
    QFutureWatcher<RStreamLoadURLData> *m_watcher;
    RStreamLoadURLData loadStreamInThread(RStreamLoadURLData &data);

};

#endif // RACKD_H
