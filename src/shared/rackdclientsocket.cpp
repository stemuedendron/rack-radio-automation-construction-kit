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


#include "rackdclientsocket.h"

#include <QHostAddress>
#include <QDataStream>


//TODO: protect against bad clients (use some protokoll header, magic string and drop everything else)
//look in qt docs QDataStream


RackdClientSocket::RackdClientSocket(QObject *parent) :
    QTcpSocket(parent),
    m_nextBlockSize(0),
    m_isAuth(false),
    m_meterPort(0)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(readyData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnectedID()));
}


void RackdClientSocket::readyData()
{

    //qDebug() << "data received from" << peerAddress().toString() << peerPort();

    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_9);

    //read all available blocks:
    forever
    {
        if (m_nextBlockSize == 0)
        {

            //qDebug() << "block size unknown - bytes available:" << bytesAvailable();

            if (bytesAvailable() < sizeof(quint32)) break;
            in >> m_nextBlockSize;
        }

        //qDebug() << "next block size for" << peerAddress().toString() << peerPort() << "is" << m_nextBlockSize;
        //qDebug() << "bytes available" << bytesAvailable();

        if (bytesAvailable() < m_nextBlockSize) break;

        //qDebug() << "complete block received";
        //qDebug() << "current block size" << m_nextBlockSize;
        //qDebug() << "bytes available" << bytesAvailable();

        //read the complete request block for later processing:
        QByteArray block = read(m_nextBlockSize);

        //qDebug() << "bytes available after read:" << bytesAvailable();

        //parse block:
        emit newBlock(this, block);
        m_nextBlockSize = 0;
    }
}


void RackdClientSocket::disconnectedID()
{
    emit disconnectedClient(this);
}

