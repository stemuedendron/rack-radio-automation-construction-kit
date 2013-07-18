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

#include "mainwindow.h"
#include "rackdclient.h"
#include <QtWidgets>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_handle(0)
{

    m_rackdClient = new RackdClient(this);
    m_le = new QLineEdit;
    m_sb = new QSpinBox;
    QPushButton *bConn = new QPushButton("connect");
    QPushButton *bPW = new QPushButton("send password");
    QPushButton *bME = new QPushButton("meter enable");
    bME->setCheckable(true);
    QPushButton *bLS = new QPushButton("load stream");
    QPushButton *bWF = new QPushButton("wave form");
    QPushButton *bPY = new QPushButton("play");
    QPushButton *bSP = new QPushButton("stop");
    QPushButton *bDC = new QPushButton("drop connection");
    m_log = new QTextEdit;

    m_slider = new QSlider(Qt::Horizontal);

    m_time = new QLabel("00:00.0");
    QFont f( "Ubuntu", 28, QFont::Bold);
    m_time->setFont(f);

    m_wave = new QLabel("(wave form)");


    connect(bConn, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(bPW, SIGNAL(clicked()), this, SLOT(sendPass()));
    connect(bME, SIGNAL(toggled(bool)), this, SLOT(meterEnable(bool)));
    connect(bLS, SIGNAL(clicked()), this, SLOT(loadStream()));
    connect(bWF, SIGNAL(clicked()), this, SLOT(waveForm()));
    connect(bPY, SIGNAL(clicked()), this, SLOT(play()));
    connect(bSP, SIGNAL(clicked()), this, SLOT(stop()));
    connect(bDC, SIGNAL(clicked()), this, SLOT(dropConnection()));

    connect(m_slider, &QSlider::sliderMoved, this, &MainWindow::setPosition);

    connect(m_rackdClient, SIGNAL(passWordOK(bool)), this, SLOT(passWordOK(bool)));
    connect(m_rackdClient, SIGNAL(streamLoaded(quint32,quint32)), this, SLOT(streamLoaded(quint32,quint32)));
    connect(m_rackdClient, SIGNAL(waveFormGenerated(quint32,QImage)), this, SLOT(waveFormGenerated(quint32,QImage)));
    connect(m_rackdClient, SIGNAL(position(quint8,quint32,quint32)), this, SLOT(position(quint8,quint32,quint32)));

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(m_le);
    l->addWidget(m_sb);
    l->addWidget(bConn);
    l->addWidget(bPW);
    l->addWidget(bME);
    l->addWidget(bLS);
    l->addWidget(bWF);
    l->addWidget(bPY);
    l->addWidget(bSP);
    l->addWidget(bDC);
    l->addWidget(m_log);
    l->addWidget(m_slider);
    l->addWidget(m_time);
    l->addWidget(m_wave);

    setLayout(l);

}

void MainWindow::connectToServer()
{
    m_rackdClient->connectToRackd(QHostAddress::Any, 1234);
}


void MainWindow::sendPass()
{
    m_rackdClient->passWord(m_le->text());
    m_le->clear();
}


void MainWindow::meterEnable(bool ok)
{
    m_rackdClient->meterEnable(ok);
}

void MainWindow::loadStream()
{
    m_rackdClient->loadStream(quint8(m_sb->value()), m_le->text());
}

void MainWindow::waveForm()
{
    m_rackdClient->waveForm(m_handle);
}


void MainWindow::play()
{
    m_rackdClient->play(m_handle);
}


void MainWindow::stop()
{
    m_rackdClient->stop(m_handle);
}


void MainWindow::dropConnection()
{
    m_rackdClient->dropConnection();
}


void MainWindow::setPosition(int pos)
{
    m_rackdClient->positionPlay(m_handle, quint32(pos));
}


//connected to signals from rackdclient:
void MainWindow::passWordOK(bool ok)
{
    ok ? m_log->append("password ok") : m_log->append("password not ok");
}

void MainWindow::streamLoaded(quint32 handle, quint32 time)
{
    m_handle = handle;
    m_slider->setMaximum(time);

}

void MainWindow::position(quint8 device, quint32 handle, quint32 position)
{
    Q_UNUSED(device);
    if (!(handle == m_handle)) return;

    QTime n(0,0,0,0);
    QTime positionTime = n.addMSecs(position);

    m_time->setText(positionTime.toString("mm:ss.z"));

    m_slider->setSliderPosition(position);

    //qDebug() << device << handle << position;

}

void MainWindow::waveFormGenerated(quint32 handle, QImage waveform)
{
    Q_UNUSED(handle);
    m_wave->setPixmap(QPixmap::fromImage(waveform));
}




