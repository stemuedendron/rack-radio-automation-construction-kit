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
#include <QImage>
#include <QDir>
#include <signal.h>

#include "rackd.h"


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
      m_maxCommandLenght(40),
      m_repositoryPath("/var/snd")
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

        if (m_auth.count() == m_maxConnections)
        {
            qDebug() << "max connections (" << m_auth.count() << ") reached";
            qDebug() << "reject client" << client->peerAddress().toString() << client->peerPort();
            client->disconnectFromHost();
            return;
        }

        m_auth.insert(client, false);
        connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
        connect(client, SIGNAL(readyRead()), this, SLOT(handleRequest()));
        connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

        qDebug() << "new client connected from" << client->peerAddress().toString() << client->peerPort();
        qDebug() << "we have now" << m_auth.count() << "connections";
    }
}


void Rackd::clientDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    if (!client) return;
    m_auth.remove(client);
    client->deleteLater();

    qDebug() << "client disconnected" << client->peerAddress().toString() << client->peerPort();
    qDebug() << "we have now" << m_auth.count() << "connections";
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

    //read the next available command:
    QByteArray command;
    if (client->canReadLine()) command.append(client->readLine());
    command = command.trimmed();

    qDebug() << "command is:" << command;


    if (command.size() > m_maxCommandLenght)
    {
        qDebug() << "ERROR: command to long!";
        return;
    }

    if (command.size() < 3)
    {
        qDebug() << "ERROR: command to short!";
        return;
    }


    if (command.endsWith("!")) command.chop(1);
    else
    {
        qDebug() << "ERROR: no valid command!";
        return;
    }

    QList<QByteArray> commandList = command.split(' ');

    /*!
    Commands have the following general syntax:

      <cmd-code> [<arg>] [...]!

    where:

        <cmd-code> A two letter command code, describing the generic action to be performed
        <arg>      Zero or more arguments, delimited by spaces or, if the last argument, by ! (see below).
        !          The ASCII character 33, indicating the end of the command sequence.

    Unless otherwise specified, the engine will echo back the command with a + or - before the !, to indicate
    the success or failure of the command execution.


    Commands without Authentication:

    PASSWORD: PW <password>!

    Pass a password to the server for authentication.

    where:

        <password> A password to be supplied before granting the client access.

    Returns: PW +! to indicate success
             PW -! to indicate failure
    */
    if (!qstrcmp(commandList[0], "PW") && commandList.size() == 2)
    {
        if (!qstrcmp(commandList[1], "letmein"))
        {
            //log
            m_auth[client] = true;
            echoCommand(client, "PW +");
            return;
        }
        else
        {
            //log
            m_auth[client] = false;
            echoCommand(client, "PW -");
            return;
        }
    }

    /*!
    DROP CONNECTION: DC!
    Drop the TCP connection and end the session.
    */
    if (!qstrcmp(commandList[0], "DC")&& commandList.size() == 1)
    {
        //log
        client->disconnectFromHost();
        return;
    }

    /*!
    Commands without Authentication:
    */
    if (!m_auth[client])
    {
        qDebug() << "ERROR: authentication required or command not exist!";
        echoCommand(client, command.append("-"));
        return;
    }

    /*!
    LOAD PLAYBACK: LP <card-num> <name>!
    Prepare an audio interface to play an audio file.
    where:
        <card-num> The number of the audio adapter to use.
        <name> The base name of an existing file in the audio storage filesystem.

    Returns: LP <card-num> <name> <stream-num> <conn-handle>!
    Where:
        <stream-num> The stream number selected to be used, or a -1 in case of error. This is relative
            to the audio adapter selected.
        <conn-handle> The connection handle. This will be used to refer to the playback event in all
             subsequent calls to rackd.

     */
    if (!qstrcmp(commandList[0], "LP") && commandList.size() == 3)
    {

        //testcode:

        if (BASS_SetDevice(commandList[1].toInt())) qDebug() << "set device ok" << commandList[1].toInt();

        HSTREAM stream = BASS_StreamCreateFile(false, qPrintable(commandList[2]), 0, 0, 0);

        command.append(' ');
        command.append(QByteArray::number(stream));
        command.append(' ');
        command.append(QByteArray::number(stream));
        echoCommand(client, command.append('+'));
        return;
    }





//    UP <conn-handle>!
//    PP <conn-handle> <position>!


//    PY <conn-handle> <length> <speed> <pitch-flag>!
    if (!qstrcmp(commandList[0], "PY") && commandList.size() == 5)
    {

        //testcode:
        if (BASS_ChannelPlay(commandList[1].toULong(), false))
        {
            qDebug() << "play ok" << commandList[1].toULong();
        }
        else
        {
            qDebug() << "play not ok" << commandList[1].toULong() << BASS_ErrorGetCode();

        }



        return;
    }



//    SP <conn-handle>!
    if (!qstrcmp(commandList[0], "SP") && commandList.size() == 2)
    {
        BASS_ChannelPause(commandList[1].toULong());
    }


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


void Rackd::echoCommand(QTcpSocket *client, QByteArray command)
{
    if (client->state() == QAbstractSocket::ConnectedState)
    {
        command.append("!\n");
        client->write(command);
    }
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
