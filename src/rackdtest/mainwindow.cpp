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
#include "rackdclient.h"
#include <QtWidgets>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_handle(0),
    m_time(0)
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

    QPushButton *bWF1 = new QPushButton("wave form1");


    QPushButton *bPY = new QPushButton("play");
    QPushButton *bSP = new QPushButton("stop");
    QPushButton *bUS = new QPushButton("unload stream");
    QPushButton *bDC = new QPushButton("drop connection");
    m_log = new QTextEdit;


    m_labelTime = new QLabel("00:00:00.0");
    QFont f( "Ubuntu", 28, QFont::Bold);
    m_labelTime->setFont(f);

    m_scene = new QGraphicsScene;
    m_scene->setBackgroundBrush(Qt::black);
    m_view = new QGraphicsView(m_scene);
   // m_view->setRenderHints(QPainter::Antialiasing);
   // m_view->setAlignment(Qt::AlignLeft);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    m_view->setDragMode(QGraphicsView::ScrollHandDrag);


    //test play cursor as QFrame:
    m_playCursor = new QFrame(m_view);
    m_playCursor->setFrameShape(QFrame::VLine);
    m_playCursor->setStyleSheet("color: gray;");

    connect(bConn, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(bPW, SIGNAL(clicked()), this, SLOT(sendPass()));
    connect(bME, SIGNAL(toggled(bool)), this, SLOT(meterEnable(bool)));
    connect(bLS, SIGNAL(clicked()), this, SLOT(loadStream()));
    connect(bWF, SIGNAL(clicked()), this, SLOT(waveForm()));

    connect(bWF1, SIGNAL(clicked()), this, SLOT(waveForm1()));

    connect(bPY, SIGNAL(clicked()), this, SLOT(play()));
    connect(bSP, SIGNAL(clicked()), this, SLOT(stop()));
    connect(bUS, SIGNAL(clicked()), this, SLOT(unloadStream()));
    connect(bDC, SIGNAL(clicked()), this, SLOT(dropConnection()));


    connect(m_view->horizontalScrollBar(), &QAbstractSlider::sliderMoved, this, &MainWindow::setPosition);


    connect(m_rackdClient, SIGNAL(passWordOK(bool)), this, SLOT(passWordOK(bool)));
    connect(m_rackdClient, SIGNAL(streamLoaded(quint32,quint32)), this, SLOT(streamLoaded(quint32,quint32)));
    connect(m_rackdClient, SIGNAL(waveFormGenerated(quint32,QImage)), this, SLOT(waveFormGenerated(quint32,QImage)));

    connect(m_rackdClient, SIGNAL(waveFormGenerated1(quint32,QList<QImage>)), this, SLOT(waveFormGenerated1(quint32,QList<QImage>)));

    connect(m_rackdClient, SIGNAL(streamUnloaded(quint32)), this, SLOT(streamUnloaded(quint32)));
    connect(m_rackdClient, SIGNAL(position(quint8,quint32,quint32)), this, SLOT(position(quint8,quint32,quint32)));

    QVBoxLayout *l = new QVBoxLayout();
    l->addWidget(m_le);
    l->addWidget(m_sb);
    l->addWidget(bConn);
    l->addWidget(bPW);
    l->addWidget(bME);
    l->addWidget(bLS);
    l->addWidget(bWF);

    l->addWidget(bWF1);

    l->addWidget(bPY);
    l->addWidget(bSP);
    l->addWidget(bUS);
    l->addWidget(bDC);
    l->addWidget(m_log);
    l->addWidget(m_labelTime);
    l->addWidget(m_view);

    setLayout(l);

}

void MainWindow::connectToServer()
{
    m_rackdClient->connectToRackd(QHostAddress::Any, 1234);
}


void MainWindow::sendPass()
{
    m_rackdClient->passWord("pass");
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

void MainWindow::waveForm1()
{
    m_rackdClient->waveForm1(m_handle);
}


void MainWindow::play()
{
    m_rackdClient->play(m_handle);
}


void MainWindow::stop()
{
    m_rackdClient->stop(m_handle);
}

void MainWindow::unloadStream()
{
    m_rackdClient->unloadStream(m_handle);

}

void MainWindow::dropConnection()
{
    m_rackdClient->dropConnection();
}


void MainWindow::setPosition(int pos)
{
    qreal scale = qreal(pos) / m_view->horizontalScrollBar()->maximum();
    m_rackdClient->positionPlay(m_handle, quint32(m_time * scale));
}


//connected to signals from rackdclient:
void MainWindow::passWordOK(bool ok)
{
    ok ? m_log->append("password ok") : m_log->append("password not ok");
}

void MainWindow::streamLoaded(quint32 handle, quint32 time)
{
    m_handle = handle;
    m_time = time;
}

void MainWindow::position(quint8 device, quint32 handle, quint32 position)
{
    Q_UNUSED(device);
    if (!(handle == m_handle)) return;

    QTime n(0,0,0,0);
    QTime positionTime = n.addMSecs(position);
    QString posStr = positionTime.toString("HH:mm:ss.zzz");
    posStr.chop(2);

    m_labelTime->setText(posStr);


    //qDebug() << device << handle << position;

    qreal scale = qreal(position) / m_time;
    m_view->horizontalScrollBar()->setValue(int(m_view->horizontalScrollBar()->maximum() * scale));

}

void MainWindow::waveFormGenerated(quint32 handle, QImage waveform)
{
    Q_UNUSED(handle);
    m_scene->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(waveform));
    m_scene->addItem(item);
    m_view->scale(1,1);   
}

void MainWindow::waveFormGenerated1(quint32 handle, QList<QImage> waveforms)
{
    Q_UNUSED(handle);
    m_scene->clear();

    for (int i = 0; i < waveforms.size(); ++i)
    {
        m_scene->addPixmap(QPixmap::fromImage(waveforms.at(i)));
    }

    QResizeEvent event(size(), size());
    QApplication::sendEvent(this, &event);
}


void MainWindow::streamUnloaded(quint32 handle)
{
    Q_UNUSED(handle);
    m_handle = 0;
    m_scene->clear();
    m_scene->setSceneRect(0,0,0,0);
    m_labelTime->setText("00:00.0");
    m_time = 0;
}


void MainWindow::resizeEvent(QResizeEvent *)
{
    m_playCursor->setGeometry(QRect(m_view->viewport()->rect().width() / 2, 1, 1, m_view->viewport()->rect().height()));

    QList<QGraphicsItem *> items = m_scene->items(Qt::AscendingOrder);
    if (items.isEmpty()) return;

    int centerX = m_view->viewport()->rect().center().x();
    int waveformWidth = 0;
    int waveformHeight = items.at(0)->boundingRect().height();

    for (int i = 0; i < items.size(); i++)
    {
        waveformWidth += items.at(i)->boundingRect().width();
        items.at(i)->setX(i * items.at(0)->boundingRect().width() + centerX);
    }

    m_scene->setSceneRect(0, 0, waveformWidth + centerX * 2, waveformHeight);
}


