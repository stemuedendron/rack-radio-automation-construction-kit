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

#include "mainwindow.h"
#include <QtWidgets>

#include "rplayer.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout *l = new QGridLayout(this);

    QList<audiodevice> devices = RPlayer::getDevices();
    foreach (const audiodevice &dev, devices)
        qDebug() << "audio device found - engine index:"
                 << dev.engineIndex << "name:"
                 << dev.name
                 << "driver:"
                 << dev.driver;

    m_device = devices[0].engineIndex;

    if (!RPlayer::init(m_device)) qWarning("Can't initialize device %d", m_device);

    //if (!RPlayer::init(3)) qWarning("Can't initialize device %d", 3);

    devices = RPlayer::getDevices();
    foreach (const audiodevice &dev, devices)
        if (dev.isInitialized)
            qDebug() << "initialized real device:"
                     << dev.engineIndex << "name:"
                     << dev.name << "driver:"
                     << dev.driver;

    for (int i=1; i < 11; i++)
    {

        QString file = qApp->applicationDirPath() + "/" + QString::number(i) + ".mp3";

        RPlayer *player = new RPlayer(this, m_device);

        QPushButton *bt = new QPushButton(file);
        //QObject::connect(bt, SIGNAL(clicked()), player, SLOT(play()));
        connect(bt,SIGNAL(clicked()),player,SLOT(playFadeOut()));

        QPushButton *fbt = new QPushButton("fade out");
        QObject::connect(fbt, SIGNAL(clicked()), player, SLOT(fadeOut()));

        QPushButton *sbt = new QPushButton("stop");
        QObject::connect(sbt, SIGNAL(clicked()), player, SLOT(stop()));

        QPushButton *pbt = new QPushButton("pause");
        QObject::connect(pbt, SIGNAL(clicked()), player, SLOT(pause()));

        QPushButton *fibt = new QPushButton("fade in");
        QObject::connect(fibt, SIGNAL(clicked()), player, SLOT(fadeIn()));

        QLabel *lb = new QLabel;
        lb->setNum(0);

        QObject::connect(player, SIGNAL(position(int)), lb, SLOT(setNum(int)));


        QLabel *lbt = new QLabel;
        lbt->setNum(0);
        QObject::connect(player, SIGNAL(time(int)), lbt, SLOT(setNum(int)));

        QPushButton *obt = new QPushButton("open");
        QObject::connect(obt,SIGNAL(clicked()),this,SLOT(openFile()));
        //QObject::connect(this,SIGNAL(setURI(QString)), player,SLOT(setURI(QString)));

        QPushButton *dbt = new QPushButton("toggle device");
        QSignalMapper *mapper = new QSignalMapper(this);
        mapper->setMapping(dbt,3);
        QObject::connect(dbt,SIGNAL(clicked()),mapper,SLOT(map()));
        QObject::connect(mapper,SIGNAL(mapped(int)),player,SLOT(setDevice(int)));


        player->setURI(file);

        l->addWidget(bt,i-1,0);
        l->addWidget(pbt,i-1,1);
        l->addWidget(sbt,i-1,2);
        l->addWidget(fibt,i-1,3);
        l->addWidget(fbt,i-1,4);
        l->addWidget(lb,i-1,5);
        l->addWidget(lbt,i-1,6);
        l->addWidget(obt,i-1,7);
        l->addWidget(dbt,i-1,8);

    }
}


void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Sound File"), "/", tr("Sound Files (*.mp3 *.mp2 *.wav)"));
    if (!(filename=="")) emit setURI(filename);

}

MainWindow::~MainWindow()
{
    if (!RPlayer::free(m_device)) qWarning("Can't release device %d", m_device);

    //if (!RPlayer::free(3)) qWarning("Can't release device %d", 3);

}
