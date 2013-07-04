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
#include <QTcpSocket>
#include <QDir>
#include <signal.h>

#include "rackd.h"

//TODO: own rack protokoll instead of caed
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


Rackd::Rackd(QObject *parent) : QTcpServer(parent),
      m_maxConnections(30),
      m_nextBlockSize(0)
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


    connect(this, SIGNAL(newConnection()), this, SLOT(clientConnected()));

}


void Rackd::clientConnected()
{

    while (hasPendingConnections())
    {
        QTcpSocket *client = nextPendingConnection();

        if (m_clients.count() == m_maxConnections)
        {
            qDebug() << "max connections (" << m_clients.count() << ") reached";
            qDebug() << "reject client" << client->peerAddress().toString() << client->peerPort();
            client->disconnectFromHost();
            return;
        }

        ClientData data;
        data.isAuth = false;

        m_clients.insert(client, data);

        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(client, SIGNAL(readyRead()), this, SLOT(handleRequest()));
        connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

        qDebug() << "new client connected from" << client->peerAddress().toString() << client->peerPort();
        qDebug() << "we have now" << m_clients.count() << "connections";

    }

    foreach (QTcpSocket *cl, m_clients.keys())
    {
        qDebug() << "active streams of" << cl << "are:"  << m_clients[cl].handleList;
    }


}


void Rackd::clientDisconnected()
{

    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (!client) return;

    //clients have to free resources (unload stream) before disconnect
    //if we have resources here we asume a client crash. so lets play until end of song
    //an then autofree stream:


    //wrong: rackd has no stream handle anymore!!!!!
    foreach (HSTREAM stream, m_clients[client].handleList)
    {
        BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING ? BASS_ChannelFlags(stream, BASS_STREAM_AUTOFREE, BASS_STREAM_AUTOFREE) : BASS_StreamFree(stream);
    }

    foreach (QTcpSocket *cl, m_clients.keys())
    {
        qDebug() << "active streams of" << cl << "are:"  << m_clients[cl].handleList;
    }



    m_clients.remove(client);
    client->deleteLater();


    qDebug() << "client disconnected" << client->peerAddress().toString() << client->peerPort();
    qDebug() << "we have now" << m_clients.count() << "connections";

}


void Rackd::displayError(QAbstractSocket::SocketError)
{
    qDebug() << errorString();
}


void Rackd::handleRequest()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (!client) return;

    qDebug() << "request received from" << client->peerAddress().toString() << client->peerPort();

    //read the next available block:
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_0);

    if (m_nextBlockSize == 0)
    {
        if (client->bytesAvailable() < sizeof(quint16)) return;
        in >> m_nextBlockSize;
    }
    if (client->bytesAvailable() < m_nextBlockSize) return;

    qDebug() << "complete block received";
    qDebug() << "current block size" << m_nextBlockSize;
    qDebug() << "bytes available" << client->bytesAvailable();


    //prepare response block:
    QByteArray response;
    QDataStream out(&response, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint16(0);

    QString command;
    in >> command;

    qDebug() << "command is:" << command;

    if (command == "PW")
    {
        QString pw;
        in >> pw;
        if (pw == "pass") //TODO: read from settings
        {
            m_clients[client].isAuth = true;
            out << command << true;
        }
        else
        {
            m_clients[client].isAuth = false;
            out << command << false;
        }
        sendBlock(client, response);
        return;
    }

    if (command == "DC")
    {
        //emit dropConnection();

        return;
    }



    qDebug() << "ERROR: unknown command" << command;
    //todo reply






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




//    if (!qstrcmp(commandList[0], "DC")&& commandList.size() == 1)
//    {
//        //log
//        client->disconnectFromHost();
//        return;
//    }

//    if (!m_clients[client].isAuth)
//    {
//        qDebug() << "ERROR: authentication required!";
//        echoCommand(client, command.append("-"));
//        return;
//    }

//    if (!qstrcmp(commandList[0], "LP") && commandList.size() == 3)
//    {

//        //test filename as uri
//        //usage: LP <card num> <uri>!
//        QString absFileName = QDir::cleanPath(commandList[2]);

//        if (BASS_SetDevice(commandList[1].toInt())) qDebug() << "set device ok" << commandList[1].toInt();

//        HSTREAM stream = BASS_StreamCreateFile(false, qPrintable(absFileName), 0, 0, 0);

//        if (stream)
//        {
//            m_clients[client].handleList.append(stream);
//            qDebug() << "client streams:" << m_clients[client].handleList;

//        }

//        qDebug() << absFileName;

//        command.append(' ');
//        command.append(QByteArray::number(stream));
//        command.append(' ');
//        command.append(QByteArray::number(stream));
//        echoCommand(client, command.append('+'));
//        return;


//    }





//    UP <conn-handle>!
//    PP <conn-handle> <position>!


//    PY <conn-handle> <length> <speed> <pitch-flag>!
//    if (!qstrcmp(commandList[0], "PY") && commandList.size() == 5)
//    {

//        //testcode:
//        if (BASS_ChannelPlay(commandList[1].toULong(), false))
//        {
//            qDebug() << "play ok" << commandList[1].toULong();
//        }
//        else
//        {
//            qDebug() << "play not ok" << commandList[1].toULong() << BASS_ErrorGetCode();

//        }



//        return;
//    }



////    SP <conn-handle>!
//    if (!qstrcmp(commandList[0], "SP") && commandList.size() == 2)
//    {
//        BASS_ChannelPause(commandList[1].toULong());
//        return;
//    }


//    //EI
//    if (!qstrcmp(commandList[0], "EI") && commandList.size() == 2)
//    {
//        qDebug() << BASS_GetConfig(BASS_CONFIG_HANDLES);
//        return;
//    }

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


}

void Rackd::sendBlock(QTcpSocket *client, QByteArray &response)
{
    if (client->state() == QAbstractSocket::ConnectedState)
    {
        QDataStream out(&response, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_0);
        out.device()->seek(0);
        out << quint16(response.size() - sizeof(quint16));

        qDebug() << "send response block:" << response.toHex() << "size (Bytes):" << response.size();

        client->write(response);
    }
    m_nextBlockSize = 0;
}



////api slots:

//void Rackd::passWord(quint64 id, const QString &passWord)
//{
//    if (passWord == "letmein")
//    {
//        m_auth[id] = true;
//        qDebug() << "client number:" << QString::number(id) << "authenticated";
//    }
//}

//void Rackd::receiveFileName(quint64 id, const QString &fileName)
//{
//    qDebug() << "receive file name:" << fileName << "from" << id;
//}


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

    foreach (int i, m_devices)
        if (BASS_SetDevice(i))
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
