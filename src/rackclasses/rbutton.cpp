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

#include "rbutton.h"
#include <qmath.h>
#include <QtGui>


//methoden verdecken (z.B. settext wenn qpushbutton ?????


RButton::RButton(ButtonKind bk, QWidget *parent)
    : QPushButton(parent),
    m_bk(bk),
    m_position(0),
    m_positionClockColor(QColor(255,255,255,100)),
    m_borderRadius(6),
    m_length(0),
    m_drawPosition(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    ////////////////////////////////////////////////////////////////
    QList<audiodevice> devices = RPlayer::getDevices();
    foreach (const audiodevice &dev, devices)
        qDebug() << "audio device found - engine index:"
                 << dev.engineIndex << "name:"
                 << dev.name
                 << "driver:"
                 << dev.driver;

    m_device = devices[0].engineIndex;

    if (!RPlayer::init(m_device)) qWarning("Can't initialize device %d", m_device);

    devices = RPlayer::getDevices();
    foreach (const audiodevice &dev, devices)
        if (dev.isInitialized)
            qDebug() << "initialized real device:"
                     << dev.engineIndex << "name:"
                     << dev.name << "driver:"
                     << dev.driver;
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////
    //QString file = qApp->applicationDirPath() + "/1.mp3";

   // QString file = ":/audio/main.ogg";

    QTemporaryFile *tmpFile = QTemporaryFile::createLocalFile(":/audio/main.ogg");
    QString file = tmpFile->fileName();

    m_player = new RPlayer(this, m_device);

    connect(this, SIGNAL(clicked()), m_player, SLOT(playFadeOut()));

    connect(m_player, SIGNAL(time(qint64)), this, SLOT(setLength(qint64)));
    connect(m_player, SIGNAL(position(qint64)), this, SLOT(setPosition(qint64)));

    setText("1.mp3");
    m_player->setURI(file);
    ////////////////////////////////////////////

}

void RButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (!m_drawPosition == 0) drawClock(&painter);
}

void RButton::refreshImage()
{
    //make sure that style sheet is reloaded to accept changed properties
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void RButton::drawClock(QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_positionClockColor);

    int radius = round(sqrt(pow(width(), 2) + pow(height(), 2))) / 2;
    QPainterPath path;
    path.addRoundedRect(1, 1, width()-1, height()-1, m_borderRadius, m_borderRadius);
    painter->setClipPath(path);

    //startangel = 90 * 16 = 1440
    painter->drawPie(width() / 2 - radius, height() / 2 - radius, 2 * radius, 2 * radius, 1440, -m_drawPosition);


    //QTime time = QTime().addMSecs(m_length - m_position);
////////////////////////////
    qint64 roundedLength = ceil(m_length/1000);
    qint64 roundedPosition = floor(m_position/1000);

    QTime time = QTime().addSecs(roundedLength - roundedPosition);

    //the same:
//    QTime totalTime = QTime().addMSecs(m_length);
//    QTime positionTime = QTime().addMSecs(m_position);
//    //remaining time:
//    QTime time = QTime().addSecs(positionTime.secsTo(totalTime));

//////////////////////////////
    QString textStatus;
    QColor colorStatus;
    QColor colorText;
    if (m_bk == bkHotKey)
    {
        colorStatus = Qt::yellow;
        textStatus = time.toString("mm:ss");
        colorText = Qt::black;
    }
    if (m_bk == bkOnAir)
    {
        colorStatus = Qt::red;
        textStatus = tr("ON AIR");
        colorText = Qt::white;
    }

    //change to use ressource font:
    QFont fontStatus(QFont("Droid Sans", 15, QFont::Bold));

    painter->setFont(fontStatus);
    painter->setBrush(colorStatus);
    painter->setPen(QPen(QColor(0,0,0,70), 4));
    QRect rectStatus = painter->boundingRect(2, 2, width()-4, height()-4, Qt::AlignCenter, textStatus);
    rectStatus.adjust(-2, 0, 4, 0);
    painter->drawRoundedRect(rectStatus, 4, 4);
    painter->setPen(QPen(colorText));
    painter->drawText(2, 2, width()-4, height()-4, Qt::AlignCenter, textStatus);
}

void RButton::resizeEvent(QResizeEvent *)
{
    refreshImage();
}

void RButton::setPosition(const qint64 &position)
{
    if (position == m_position) return;
    m_position = position;

    if (m_length == 0) return;
    //in QPainter::drawPie a full circle equals 5760 (16 * 360)
    int pos = m_position * 5760 / m_length;
    if (m_drawPosition == pos) return;
    m_drawPosition = pos;

    refreshImage();
}

void RButton::setPositionClockColor(const QColor &color)
{
    if (color == m_positionClockColor) return;
    m_positionClockColor = color;
    refreshImage();
}


void RButton::setBorderRadius(const int &borderRadius)
{
    if (borderRadius == m_borderRadius) return;
    m_borderRadius = borderRadius;
    refreshImage();
}

void RButton::setLength(const qint64 &length)
{
    if (length == m_length) return;
    m_length = length;
    refreshImage();
}

RButton::~RButton()
{
    if (RPlayer::free(m_device)) qDebug("free device with engine index: %d", m_device);
    else qWarning("Can't release device %d", m_device);
}



