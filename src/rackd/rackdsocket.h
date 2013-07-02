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


#ifndef RACKDSOCKET_H
#define RACKDSOCKET_H

#include <QTcpSocket>
#include <QDataStream>


class RackdSocket : public QTcpSocket
{

    Q_OBJECT

public:

    explicit RackdSocket(QObject *parent = 0);
    bool isAuth() const {return m_isAuth;}
    void setAuth(bool ok) {m_isAuth = ok;}



public slots:

    void passWord(const QString &password, bool ok);
//    void dropConnection();
    void loadStream(quint8 device, const QString &uri, quint32 handle, bool ok);

signals:

    void onPassWord(const QString &password, bool ok);
//    void onDropConnection();
//    void onLoadStream(quint8 device, const QString &uri, quint32 handle, bool ok);


private slots:

    void readData();

private:

    void sendBlock();
    RSocketType m_socketType;
    quint16 m_nextBlockSize;
    QByteArray m_blockToSend;
    QDataStream m_outStream;
    bool m_isAuth;

};

#endif // RACKDSOCKET_H
