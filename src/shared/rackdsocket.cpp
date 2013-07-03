
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


#include "rackdsocket.h"


RackdSocket::RackdSocket(QObject *parent)
    : QTcpSocket(parent),
      m_nextBlockSize(0),
      m_outStream(&m_blockToSend, QIODevice::WriteOnly)
{
    m_outStream.setVersion(QDataStream::Qt_5_0);
    m_outStream << quint16(0);
    connect(this, SIGNAL(readyRead()), this, SLOT(handleResponse()));
}


void RackdSocket::passWord(const QString &password)
{
    m_outStream << QString("PW") << password;
    sendCommand();
}


void RackdSocket::loadStream(quint8 device, const QString &uri)
{

}

void RackdSocket::sendCommand()
{
    if (state() == QAbstractSocket::ConnectedState)
    {
        m_outStream.device()->seek(0);
        m_outStream << quint16(m_blockToSend.size() - sizeof(quint16));
        write(m_blockToSend);
    }
    m_blockToSend.clear();
}

void RackdSocket::handleResponse()
{

    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_0);

    if (m_nextBlockSize == 0)
    {
        if (bytesAvailable() < sizeof(quint16)) return;
        in >> m_nextBlockSize;
    }

    if (bytesAvailable() < m_nextBlockSize) return;

    QString command;
    in >> command;

    if (command == "PW")
    {
        bool ok;
        in >> ok;
        emit passWord(ok);
        return;
    }

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

    m_nextBlockSize = 0;

}





