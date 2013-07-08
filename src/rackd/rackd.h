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


#include "rmessage.h"

class RackdClientSocket;

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
    void handleRequest(RackdClientSocket *client, const QByteArray &requestBlock);

    //loadStreamURL thread finished slot:
    void loadStreamFinished();


private:

    //protocol handling:
    void sendResponse(RackdClientSocket *client);

    quint16 m_maxConnections;


    // delete: ?????
    QByteArray m_response;

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

    //QFutureWatcher<QHash<RackdClientSocket *, QByteArray> > *m_watcher;
    //QHash<RackdClientSocket *, QByteArray> loadStreamThread(RackdClientSocket *client, const QString &command, quint8 device, const QString &uri);


    QFutureWatcher<RMessage> *m_watcher;
    RMessage loadStreamThread(RackdClientSocket *client, const QString &command, quint8 device, const QString &uri);


};

#endif // RACKD_H
