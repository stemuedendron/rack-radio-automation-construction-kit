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


RackdClient::RackdClient(QObject *parent)
    : QObject(parent),
      m_nextBlockSize(0)
{
    m_tcpSocket = new QTcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(m_tcpSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(handleResponse()));

}

//connection handling:

void RackdClient::connectToRackd(const QHostAddress &address, quint16 port)
{
    m_tcpSocket->connectToHost(address, port);
}

void RackdClient::disconnectFromRackd()
{
    m_tcpSocket->disconnectFromHost();
}

//TODO:
void RackdClient::handleError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}


void RackdClient::sendBlock()
{
    if (m_tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out << quint16(0);

        foreach (const QVariant &value, m_request)
        {
            switch (value.type())
            {
            case QMetaType::QString: out << value.toString(); break;
            case QMetaType::Bool: out << value.toBool(); break;
            case QMetaType::Int: out << value.toInt();  break;
            case QMetaType::UInt: out << value.toUInt(); break;
            case QMetaType::ULongLong: out << value.toULongLong(); break;
            default: out << value;
            }
        }

        out.device()->seek(0);
        out << quint16(block.size() - sizeof(quint16));

        //debug:
        qDebug() << "send request:";
        foreach (const QVariant &value, m_request)
        {
            qDebug() << value;
        }
        qDebug() << "request block:" << block.toHex() << "size (Bytes):" << block.size();

        m_tcpSocket->write(block);
    }
    m_request.clear();
}


//rackd protocoll implementation, requests:

void RackdClient::passWord(const QString &password)
{
    m_request.append(QString("PW"));
    m_request.append(password);
    sendBlock();
}

void RackdClient::dropConnection()
{
    m_request.append(QString("DC"));
    sendBlock();
}

void RackdClient::loadStream(quint8 device, const QString &uri)
{
    m_request.append(QString("LS"));
    m_request.append(device);
    m_request.append(uri);
    sendBlock();
}

void RackdClient::unloadStream(quint32 handle)
{
    m_request.append(QString("US"));
    m_request.append(handle);
    sendBlock();
}

void RackdClient::positionPlay(quint32 handle, quint32 pos)
{
    m_request.append(QString("PP"));
    m_request.append(handle);
    m_request.append(pos);
    sendBlock();
}

void RackdClient::play(quint32 handle)
{
    m_request.append(QString("PY"));
    m_request.append(handle);
    sendBlock();
}

void RackdClient::stop(quint32 handle)
{
    m_request.append(QString("SP"));
    m_request.append(handle);
    sendBlock();
}




//rackd protocoll implementation, response:
void RackdClient::handleResponse()
{

    qDebug() << "got response from server";

    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_5_0);
    if (m_nextBlockSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < sizeof(quint16)) return;
        in >> m_nextBlockSize;
    }
    if (m_tcpSocket->bytesAvailable() < m_nextBlockSize) return;
    QString command;
    in >> command;

    if (command == "PW")
    {
        bool ok;
        in >> ok;

        qDebug() << "authentificated:" << ok;

        emit passWordOK(ok);
        m_nextBlockSize = 0;
        return;
    }

    qDebug() << "ERROR: unknown response" << command;
    m_nextBlockSize = 0;

//    if (command == "DC")
//    {
//        emit dropConnection();
//        return;
//    }


//    if (!m_isAuth)
//    {
//        qDebug() << "ERROR: authentication required!";
//        //todo reply
//        //echoCommand(client, command.append("-"));
//        return;
//    }



//    if (command == "LS")
//    {
//        quint8 device;
//        QString uri;
//        in >> device >>uri;
//        emit loadStream(device, uri);
//        return;
//    }

//    if (command == "PP")
//    {
//        quint32 handle;
//        quint32 position;
//        in >> handle >> position;
//        emit playPosition(handle, position);
//        return;
//    }

//    if (command == "PY")
//    {
//        quint32 handle;
//        quint32 length;
//        quint16 speed;
//        bool pitch;
//        in >> handle >> length >> speed >> pitch;
//        emit play(handle, length, speed, pitch);
//        return;
//    }

//    if (command == "SP")
//    {
//        quint32 handle;
//        in >> handle;
//        emit stop(handle);
//        return;
//    }


}














