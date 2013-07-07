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


#include <QCoreApplication>
#include <QDir>
#include <signal.h>

#include "rackd.h"
#include "rackdclientsocket.h"

//TODO: if connections count is null go to auto modus ???
//TODO: qsettings
//loglevels?


void SigHandler(int signum)
{
  switch(signum)
  {
      case SIGINT:
      case SIGTERM:
      case SIGHUP:
      qDebug() << "signal:" << signum << "received";
        qApp->quit();
      break;
  }
}


Rackd::Rackd(QObject *parent)
    : QTcpServer(parent),
      m_maxConnections(3)
{
    qDebug() << "Hello from rack daemon!";

    signal(SIGHUP,SigHandler);
    signal(SIGINT,SigHandler);
    signal(SIGTERM,SigHandler);


    //audio devices:
    BASS_DEVICEINFO info;
    for (int i = 1; BASS_GetDeviceInfo(i, &info); i++) if (info.flags&BASS_DEVICE_ENABLED)
    {
        if (QString::compare(info.name,"Default") == 0) continue;

        qDebug() << i << "audio device name:" << info.name;
        qDebug() << "audio device driver:" << info.driver;
        if (BASS_Init(i,44100,0,0,NULL))
        {
            qDebug() << info.driver << "initialized";
            m_devices.append(i);
        }
    }




    if (!listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Unable to start tcp server!";
        qDebug() << "Error is:" << errorString();
        qDebug() << "rack deamon exiting";
        exit(1);
    }
    else
    {
        qDebug() << "waiting for incoming connections..." << serverAddress().toString() << serverPort();
    }

}


void Rackd::incomingConnection(qintptr socketId)
{

    qDebug() << "new incomming connection";

    if (m_clients.count() == m_maxConnections)
    {
        qDebug() << "max connections (" << m_clients.count() << ") reached";
        qDebug() << "reject client";
        RackdClientSocket *dropClient = new RackdClientSocket(this);
        dropClient->setSocketDescriptor(socketId);
        connect(dropClient, SIGNAL(disconnected()), dropClient, SLOT(deleteLater()));
        dropClient->disconnectFromHost();
        return;
    }

    RackdClientSocket *client = new RackdClientSocket(this);
    client->setSocketDescriptor(socketId);

    m_clients.append(client);

    connect(client, SIGNAL(newBlock(RackdClientSocket*,QByteArray)), this, SLOT(handleRequest(RackdClientSocket*,QByteArray)));
    connect(client, SIGNAL(disconnectedClient(RackdClientSocket*)), this, SLOT(clientDisconnected(RackdClientSocket*)));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

    qDebug() << "new client connected from" << client->peerAddress().toString() << client->peerPort();
    qDebug() << "we have now" << m_clients.count() << "connections";

}


void Rackd::clientDisconnected(RackdClientSocket *client)
{
    //clients have to free resources (unload streams) before disconnect
    //if we have resources here we asume a client crash or drop connection. so lets play until end of song
    //an then autofree stream:


    //wrong: rackd has no stream handle anymore!!!!!
    //change this: handle list must be a rackd member


//    foreach (HSTREAM stream, m_clients[client].handleList)
//    {
//        BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING ? BASS_ChannelFlags(stream, BASS_STREAM_AUTOFREE, BASS_STREAM_AUTOFREE) : BASS_StreamFree(stream);
//    }

//    foreach (QTcpSocket *cl, m_clients.keys())
//    {
//        qDebug() << "active streams of" << cl << "are:"  << m_clients[cl].handleList;
//    }

    m_clients.removeAll(client);
    client->deleteLater();

    qDebug() << "client disconnected" << client->peerAddress().toString() << client->peerPort();
    qDebug() << "we have now" << m_clients.count() << "connections";

}

//TODO:
void Rackd::handleError(QAbstractSocket::SocketError)
{
    qDebug() << errorString();
}


//rackd protocoll implementation:

void Rackd::handleRequest(RackdClientSocket *client, const QByteArray &requestBlock)
{

    QDataStream request(requestBlock);
    request.setVersion(QDataStream::Qt_5_0);

    //prepare response block:
    QDataStream out(&m_response, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0);

    QString command;
    request >> command;

    qDebug() << "command is:" << command;

    if (command == "PW")
    {
        QString pw;
        request >> pw;
        if (pw == "pass") //TODO: read from settings
        {
            client->setAuth(true);
            out << command << true;
        }
        else
        {
            client->setAuth(false);
            out << command << false;
        }

        qDebug() << "client" <<  client->peerAddress().toString()  << client->peerPort() << "authenticated:" << client->isAuth();

        out.device()->seek(0);
        out << quint16(m_response.size() - sizeof(quint16));
        sendResponse(client);
        return;
    }

    if (command == "DC")
    {
        client->disconnectFromHost();
        return;
    }


    //the following requests needs authentication:

    if (!client->isAuth())
    {

        qDebug() << "ERROR: authentication required!";

        out << QString("ER") << command << QString("request need authentication");
        out.device()->seek(0);
        out << quint16(m_response.size() - sizeof(quint16));
        sendResponse(client);
        return;
    }

    if (command == "LS")
    {
        quint8 device;
        QString uri;
        request >> device >> uri;

        qDebug() << uri;

        BASS_SetDevice(device);
        HSTREAM handle;
        if (uri.startsWith("http://", Qt::CaseInsensitive) || uri.startsWith("ftp://", Qt::CaseInsensitive))
        {
            handle = BASS_StreamCreateURL(qPrintable(uri), 0, 0, NULL, 0);
        }
        else
        {
            QString absFileName = QDir::cleanPath(uri);
            handle = BASS_StreamCreateFile(false, qPrintable(absFileName), 0, 0, 0);
        }

        if (handle)
        {


            //////////////////////////////////////////

            //m_clients[client].handleList.append(handle);


            qDebug() << "load stream:" << uri;

            //qDebug() << "client streams:" << m_clients[client].handleList;

            out << command << device << uri << quint32(handle) << true;
        }
        else
        {
            qDebug() << "ERROR: load stream failed:" << uri << BASS_ErrorGetCode();
            out << command << device << uri << quint32(handle) << false;
        }

        out.device()->seek(0);
        out << quint16(m_response.size() - sizeof(quint16));
        sendResponse(client);
        return;
    }



    //    if (command == "PP")
    //    {
    //        quint32 handle;
    //        quint32 position;
    //        in >> handle >> position;
    //        emit playPosition(handle, position);
    //        return;
    //    }



    if (command == "PY")
    {
        quint32 handle;
        //quint32 length;
        //quint16 speed;
        //bool pitch;
        request >> handle;
        if (BASS_ChannelPlay(handle, false))
        {
            qDebug() << "play ok:" << handle;
            out << command << handle << true;
        }
        else
        {
            qDebug() << "play not ok:" << handle << BASS_ErrorGetCode();
            out << command << handle << false;
        }

        out.device()->seek(0);
        out << quint16(m_response.size() - sizeof(quint16));
        sendResponse(client);
        return;
    }


    if (command == "SP")
    {
        quint32 handle;
        request >> handle;
        if (BASS_ChannelPause(handle))
        {
            qDebug() << "stop ok:" << handle;
            out << command << handle << true;
        }
        else
        {
            qDebug() << "stop not ok:" << handle << BASS_ErrorGetCode();
            out << command << handle << false;
        }

        out.device()->seek(0);
        out << quint16(m_response.size() - sizeof(quint16));
        sendResponse(client);
        return;
    }



    //    //EI
    //    if (!qstrcmp(commandList[0], "EI") && commandList.size() == 2)
    //    {
    //        qDebug() << BASS_GetConfig(BASS_CONFIG_HANDLES);
    //        return;
    //    }

    //    UP <conn-handle>!
    //    PP <conn-handle> <position>!
    //    TS <card-num>!
    //    LR <card-num> <port-num> <coding> <channels> <samp-rate> <bit-rate>
    //    UR <card-num> <stream-num>!
    //    RD <card-num> <stream-num> <length> <threshold>!
    //    RS <card-num> <stream-num>!
    //    SR <card-num> <stream-num>!
    //    IV <card-num> <stream-num> <level>!
    //    OV <card-num> <stream-num> <port-num> <level>!
    //    FV <card-num> <stream-num> <port-num> <level> <length>!
    //    IL <card-num> <port-num> <level>!
    //    OL <card-num> <port-num> <level>!
    //    IM <card-num> <stream-num> <mode>!
    //    OM <card-num> <stream-num> <model>!
    //    IX <card-num> <stream-num> <level>!
    //    IT <card-num> <port-num> <type>!
    //    IS <card-num> <port-num>!
    //    AL <card-num> <input-num> <output-num> <level>!
    //    CO <card-num> <port-num> <udp-port> <samp-rate> <chans>!
    //    JC <output_name> | <input_name>!
    //    JD <output_name> | <input_name>!
    //    ME <udp-port>!
    //    ML <type> <card-num> <port-num> <left-lvl> <right-lvl>!
    //    MO <card-num> <stream-num> <left-lvl> <right-lvl>!
    //    MP <card-num> <stream-num> <pos>!
    //    MS <card-num> <port-num> <stream-num> <status>!


    qDebug() << "ERROR: unknown request" << command;
    out << QString("ER") << command << QString("unknown request");
    out.device()->seek(0);
    out << quint16(m_response.size() - sizeof(quint16));
    sendResponse(client);

}


void Rackd::sendResponse(RackdClientSocket *client)
{
    if (client->state() == QAbstractSocket::ConnectedState)
    {

        qDebug() << "send response block:" << m_response.toHex() << "size (Bytes):" << m_response.size();

        client->write(m_response);
    }
    m_response.clear();
}

//void Rackd::getWaveForm(quint64 id, const qulonglong itemID)
//{

//    if (itemID > 9999999999)
//    {
//        qDebug() << "id out of range";
//        return;
//    }

//    qDebug() << "waveform image requested from client" << id << "for item id" << itemID;

//    QString itemIDStr = QString::number(itemID);
//    itemIDStr = itemIDStr.rightJustified(10, '0');

//    //QString absFileName;
//    //absFileName = "/home/rf/Documents/qt-projects/00-Steffen/rack-network-test/build-release/rackd";

//    QString absFileName(QDir::currentPath());

//    absFileName.append("/repo");

//    absFileName.append("/");
//    absFileName.append(itemIDStr.mid(0,2));
//    absFileName.append("/");
//    absFileName.append(itemIDStr.mid(2,2));
//    absFileName.append("/");
//    absFileName.append(itemIDStr.mid(4,2));
//    absFileName.append("/");
//    absFileName.append(itemIDStr.mid(6,2));
//    absFileName.append("/");
//    absFileName.append(itemIDStr);
//    absFileName.append(".png");

//    qDebug() << "absolute file:" << absFileName;

//    QImage wavForm;
//    if (!wavForm.load(absFileName))
//    {

//        qDebug() << "can not load waveform image" << absFileName;
//        //todo: send fail message
//        return;

//    }
//    qDebug() << "send waveform image" << absFileName;
//    m_rpcPeer->call(id, "waveForm", wavForm);

//}



void Rackd::doCleanUp()
{
    qDebug() << "clean up called";
    qDebug() << "stop listen for incoming connections";
    close();

    foreach (int i, m_devices) if (BASS_SetDevice(i))
    {
        BASS_DEVICEINFO info;
        BASS_GetDeviceInfo(i, &info);
        if (BASS_Free()) qDebug() << "audio device" << info.driver << "deinitialized";
    }

}


Rackd::~Rackd()
{
    qDebug() << "destructor called. Goodbye from rack daemon!";
}
