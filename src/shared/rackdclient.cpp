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


#include "rackdclient.h"
#include "rackdclientsocket.h"

#include <QDataStream>
#include <QUdpSocket>


RackdClient::RackdClient(QObject *parent)
    : QObject(parent),
      m_socket(new RackdClientSocket(this)),
      m_meterSocket(new QUdpSocket(this)),
      m_requestDS(new QDataStream(&m_request, QIODevice::WriteOnly))
{

    m_requestDS->setVersion(QDataStream::Qt_4_9);
    *m_requestDS << quint32(0);

    for (quint16 i = 30000; i < 30100; i++)
    {
        if (m_meterSocket->bind(i))
        {
            qDebug() << "udp meter socket bound to port:" << m_meterSocket->localPort();
            connect(m_meterSocket, SIGNAL(readyRead()), this, SLOT(handleDatagram()));
            connect(m_meterSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
            break;
        }
    }

    //TODO: change this to get status info from socket (peer address and port)
    connect(m_socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

    connect(m_socket, SIGNAL(newBlock(RackdClientSocket*, QByteArray)), this, SLOT(handleResponse(RackdClientSocket*, QByteArray)));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

}

RackdClient::~RackdClient()
{
    delete m_requestDS;
}


//connection handling:

void RackdClient::connectToRackd(const QHostAddress &address, quint16 port)
{    
    qDebug() << "connect to rackd:" << address.toString() << port;
    m_socket->connectToHost(address, port);
}


void RackdClient::disconnectFromRackd()
{
    qDebug() << "disconnect from rackd";
    m_socket->disconnectFromHost();
}


//TODO:
void RackdClient::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}








//rackd protocoll implementation, requests:

void RackdClient::passWord(const QString &password)
{
    *m_requestDS << QString("PW") << password;
    sendRequest();
}

void RackdClient::dropConnection()
{
    *m_requestDS << QString("DC");
    sendRequest();
}

void RackdClient::loadStream(quint8 device, const QString &uri)
{
    *m_requestDS << QString("LS") << device << uri;
    sendRequest();
}

void RackdClient::unloadStream(quint32 handle)
{
    *m_requestDS << QString("US") << handle;
    sendRequest();
}

void RackdClient::positionPlay(quint32 handle, quint32 pos)
{
    *m_requestDS << QString("PP") << handle << pos;
    sendRequest();
}

void RackdClient::play(quint32 handle)
{
    *m_requestDS << QString("PY") << handle;
    sendRequest();
}

void RackdClient::stop(quint32 handle)
{
    *m_requestDS << QString("SP") << handle;
    sendRequest();
}

void RackdClient::meterEnable(bool ok)
{
    ok ? *m_requestDS << QString("ME") << m_meterSocket->localPort() : *m_requestDS << QString("ME") << quint16(0);
    sendRequest();
}

void RackdClient::waveForm(quint32 handle)
{
    *m_requestDS << QString("WF") << handle;
    sendRequest();
}

void RackdClient::waveForm1(quint32 handle)
{
    *m_requestDS << QString("WL") << handle;
    sendRequest();
}


void RackdClient::sendRequest()
{
    m_requestDS->device()->seek(0);
    *m_requestDS << quint32(m_request.size() - sizeof(quint32));
    if (m_socket->state() == QAbstractSocket::ConnectedState)
    {
        //qDebug() << "send request block:" << m_request.toHex() << "size (Bytes):" << m_request.size();
        m_socket->write(m_request);
    }
    m_request.clear();
}








//TODO: check if we got a complete response (end response marker field)
//      and if not report error ????

//rackd protocoll implementation, response:

void RackdClient::handleResponse(RackdClientSocket *client, const QByteArray &responseBlock)
{

    //qDebug() << "received response block:" << responseBlock.toHex() << "size (Bytes):" << responseBlock.size();

    Q_UNUSED(client);
    QDataStream response(responseBlock);
    response.setVersion(QDataStream::Qt_4_9);

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

        if (ok) emit streamLoaded(handle, time);
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

        qDebug() << "seek:" << handle << position << ok;

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



    if (command == "ME")
    {
        quint16 port;
        bool ok;
        response >> port >> ok;

        qDebug() << "meter enable:" << port << ok;

        //TODO: do something here like start timer????


        return;
    }


    if (command == "WF")
    {
        quint32 handle;
        QImage waveform;
        bool ok;
        response >> handle >> waveform >> ok;

        qDebug() << "wave form:" << handle << ok;

        if (ok) emit waveFormGenerated(handle, waveform);
        return;
    }


    if (command == "WL")
    {
        quint32 handle;
        QList<QImage> waveforms;
        bool ok;
        response >> handle >> waveforms >> ok;

        qDebug() << "wave form1:" << handle << ok;

        if (ok) emit waveFormGenerated1(handle, waveforms);
        return;
    }


    qDebug() << "ERROR: unknown response" << command;

}


//udp meter status:
void RackdClient::handleDatagram()
{
    QByteArray datagram;

    do
    {
        datagram.resize(m_meterSocket->pendingDatagramSize());
        m_meterSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream response(datagram);
        response.setVersion(QDataStream::Qt_4_9);

        QString command;
        response >> command;

        if (command == "MP")
        {
            quint8 device;
            quint32 handle;
            quint32 pos;
            quint16 leftLevel;
            quint16 rightLevel;

            response >> device >> handle >> pos >> leftLevel >> rightLevel;

            //qDebug() << "meter position:" << device << handle << pos;

            emit position(device, handle, pos, leftLevel, rightLevel);
        }

    }
    while (m_meterSocket->hasPendingDatagrams());


    //TODO: ohter status commands...

}




