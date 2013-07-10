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


#ifndef RACKDCLIENT_H
#define RACKDCLIENT_H

#include <QObject>
#include <QHostAddress>

class RackdClientSocket;

class RackdClient : public QObject
{

    Q_OBJECT

public:

    explicit RackdClient(QObject *parent = 0);


public slots:

    //connection handling:
    void connectToRackd(const QHostAddress & address, quint16 port);
    void disconnectFromRackd();

    //protocoll implementation, requests:
    void passWord(const QString &password);
    void dropConnection();

    void loadStream(quint8 device, const QString &uri);
    void unloadStream(quint32 handle);
    void positionPlay(quint32 handle, quint32 pos);
    void play(quint32 handle);
    void stop(quint32 handle);


private slots:

    //connection handling:
    void handleError(QAbstractSocket::SocketError socketError);

    //protocol handling:
    void handleResponse(RackdClientSocket* client, const QByteArray &responseBlock);


signals:

    //connection handling:
    void connected();
    void disconnected();

    //protocoll implementation, responses:
    void passWordOK(bool ok);

    void streamLoaded(quint32 handle);
    void streamUnloaded(quint32 handle);
    void playPositioned(quint32 handle, quint32 position);
    void playing(quint32 handle);
    void stopped(quint32 handle);

    void streamTime(quint32 time);


private:

    RackdClientSocket *m_socket;
    QByteArray m_request;
    void sendRequest();

};

#endif // RACKDCLIENT_H

