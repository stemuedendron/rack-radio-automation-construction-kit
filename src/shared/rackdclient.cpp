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


#include "rackdclient.h"
#include "rackdclientsocket.h"
#include <QDataStream>


RackdClient::RackdClient(QObject *parent)
    : QObject(parent)
{
    m_socket = new RackdClientSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(m_socket, SIGNAL(newBlock(RackdClientSocket*, QByteArray)), this, SLOT(handleResponse(RackdClientSocket*, QByteArray)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
}


//connection handling:

void RackdClient::connectToRackd(const QHostAddress &address, quint16 port)
{    
    m_socket->connectToHost(address, port);
}


void RackdClient::disconnectFromRackd()
{
    m_socket->disconnectFromHost();
}


//TODO:
void RackdClient::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}


void RackdClient::sendRequest()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "send request block:" << m_request.toHex() << "size (Bytes):" << m_request.size();

        m_socket->write(m_request);
    }
    m_request.clear();
}


//rackd protocoll implementation, requests:

void RackdClient::passWord(const QString &password)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("PW") << password;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::dropConnection()
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("DC");
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::loadStream(quint8 device, const QString &uri)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("LS") << device << uri;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::unloadStream(quint32 handle)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("US") << handle;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::positionPlay(quint32 handle, quint32 pos)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("PP") << handle << pos;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::play(quint32 handle)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("PY") << handle;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}

void RackdClient::stop(quint32 handle)
{
    QDataStream out(&m_request, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0) << QString("SP") << handle;
    out.device()->seek(0);
    out << quint16(m_request.size() - sizeof(quint16));
    sendRequest();
}



//TODO: check if we got a complete response (end response marker field)
//      and if not report error ????

//rackd protocoll implementation, response:

void RackdClient::handleResponse(RackdClientSocket *client, const QByteArray &responseBlock)
{
    Q_UNUSED(client);
    QDataStream response(responseBlock);
    response.setVersion(QDataStream::Qt_5_0);

    QString command;
    response >> command;

    if (command == "ER")
    {
        QString request;
        QString message;
        response >> request >> message;

        qDebug() << "ERROR:" << request << message;

        return;
    }


    if (command == "PW")
    {
        bool ok;
        response >> ok;

        qDebug() << "authentificated:" << ok;

        emit passWordOK(ok);
        return;
    }


    if (command == "DC")
    {

        qDebug() << "connection dropped";

        emit dropConnection();
        return;
    }


    if (command == "LS")
    {
        quint8 device;
        QString uri;
        quint32 handle;
        quint32 time;
        bool ok;
        response >> device >> uri >> handle >> time >> ok;

        qDebug() << "load stream:" << device << uri << handle << time << ok;

        if (ok)
        {
            emit streamLoaded(handle);
            emit streamTime(time);
        }
        return;
    }

    if (command == "US")
    {
        quint32 handle;
        bool ok;
        response >> handle >> ok;

        qDebug() << "unload stream:" << handle << ok;

        if (ok) emit streamUnloaded(handle);
        return;
    }

    if (command == "PP")
    {
        quint32 handle;
        quint32 position;
        bool ok;
        response >> handle >> position >> ok;

        qDebug() << "seek ok:" << handle << position;

        if (ok) emit playPositioned(handle, position);
        return;
    }

    if (command == "PY")
    {
        quint32 handle;
        //quint32 length;
        //quint16 speed;
        //bool pitch;
        bool ok;
        response >> handle >> ok;

        qDebug() << "play:" << handle << ok;

        if (ok) emit playing(handle);
        return;
    }

    if (command == "SP")
    {
        quint32 handle;
        bool ok;
        response >> handle >> ok;

        qDebug() << "stop:" << handle << ok;

        if (ok) emit stopped(handle);
        return;
    }

    qDebug() << "ERROR: unknown response" << command;

}














