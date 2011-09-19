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

#include "rplayer.h"


//FIXME: bass must be initialized before
//should we check this by a member like bass_engine?

//TODO: more signals

//TODO: looping (see bass docs to BASS_ChannelSetSync)

//per config for bass usable devices and only those can initialized (be fair to other apps :-)

//bass engine member
//inherited classes for stream sample url or change setURI?

//maybe the timer in user code?? look at the tick concept in phonon

//look for usefull members/signals etc to phonon::mediaobject

/*for time stuff:

from phonon mediaplayer example:

void MediaPlayer::updateTime()
 {
     long len = m_MediaObject.totalTime();
     long pos = m_MediaObject.currentTime();
     QString timeString;
     if (pos || len)
     {
         int sec = pos/1000;
         int min = sec/60;
         int hour = min/60;
         int msec = pos;

         QTime playTime(hour%60, min%60, sec%60, msec%1000);
         sec = len / 1000;
         min = sec / 60;
         hour = min / 60;
         msec = len;

         QTime stopTime(hour%60, min%60, sec%60, msec%1000);
         QString timeFormat = "m:ss";
         if (hour > 0)
             timeFormat = "h:mm:ss";
         timeString = playTime.toString(timeFormat);
         if (len)
             timeString += " / " + stopTime.toString(timeFormat);
     }
     timeLabel->setText(timeString);
 }


from simple media player:

void MainWindow::tick(qint64 time)
{
    QTime t = QTime().addMSecs(time);
    lTime->setText(t.toString((time >= 3600000) ? "h:mm:ss" : "m:ss"));
}





*/

bool RPlayer::init(const int &device)
{
    if (HIWORD(BASS_GetVersion())!=BASSVERSION)
    {
        qDebug("An incorrect version of BASS library was loaded");
        return 0;
    }

    if (!BASS_Init(device, 44100, 0, 0, NULL))
    {
        qDebug("Can not init BASS library on device with engine index %d", device);
        return false;
    }

    qDebug("device with engine index %d initialized", device);
    return true;
}

bool RPlayer::free(const int &device)
{
    BASS_SetDevice(device);
    return (BASS_Free());
}


QList<audiodevice> RPlayer::getDevices()
{
    QList<audiodevice> devices;
    BASS_DEVICEINFO info;
    for (int i = 1; BASS_GetDeviceInfo(i, &info); i++) if (info.flags&BASS_DEVICE_ENABLED)
    {
        if (QString::compare(info.name,"Default") == 0) continue;
        audiodevice dev;
        dev.engineIndex = i;
        dev.isInitialized = info.flags&BASS_DEVICE_INIT;
        dev.name = QString(info.name);
        dev.driver = QString(info.driver);
        devices.append(dev);
    }
    return devices;
}



void CALLBACK stopSyncProc(HSYNC, DWORD, DWORD, void *user)
{
    RPlayer* pl = (RPlayer*)user;
    pl->stop();
}

RPlayer::RPlayer(QObject *parent, const int &device)
    : QObject(parent),
    m_device(device),
    m_state(DisabledState),
    m_volume(1),
    m_fadeInTime(2000),
    m_fadeOutTime(2000),
    m_isFadingOut(false)

{
    emit stateChanged();
}

RPlayer::~RPlayer()
{
    BASS_StreamFree(m_stream);
}

void RPlayer::setDevice(const int &device)
{
    if (device == m_device) return;
    if (BASS_ChannelSetDevice(m_stream, device)) m_device = device;
}

void RPlayer::setURI(const QString &uri)
{
    //FIXME maybe this is wrong! if a plugin load its audio files while no device is available,
    //it fails! we should change the device context somewhere else (channelsetdevice?)

    //other way: the plugin first must check the device and then load its audio, maybe better?

    if (BASS_SetDevice(m_device))
    {
        if (m_stream) BASS_StreamFree(m_stream);
        m_stream = BASS_StreamCreateFile(false, qPrintable(uri), 0, 0, 0);
        if (m_stream)
        {
            BASS_ChannelSetSync(m_stream, BASS_SYNC_END, 0, &stopSyncProc, this);
            emit time(qint64(BASS_ChannelBytes2Seconds(m_stream, BASS_ChannelGetLength(m_stream, BASS_POS_BYTE))*1000));
            m_state = StoppedState;
        }
        else m_state = ErrorState;
        emit stateChanged();
    }
}

void RPlayer::setVolume(const float &vol)
{
    m_volume = vol;
    BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_VOL, m_volume);
}

void RPlayer::setFadeInTime(const DWORD &time)
{
    m_fadeInTime = time;
}

void RPlayer::setFadeOutTime(const DWORD &time)
{
    m_fadeOutTime = time;
}

void RPlayer::play()
{
    if (BASS_ChannelPlay(m_stream, false))
    {
        m_timer.start(50, this);
        m_state = PlayingState;
    }
    else m_state = ErrorState;
    emit stateChanged();
}

void RPlayer::pause()
{
    if (BASS_ChannelPause(m_stream))
    {
        m_state = PausedState;
        emit stateChanged();
    }
}

void RPlayer::stop()
{
    if (BASS_ChannelStop(m_stream))
    {
        BASS_ChannelSetPosition(m_stream, 0, 0);
        BASS_ChannelSlideAttribute(m_stream, BASS_ATTRIB_VOL, m_volume, 0);
        m_isFadingOut = false;
        emit position(0);
        m_state = StoppedState;
    }
    else m_state = ErrorState;
    emit stateChanged();
}

void RPlayer::fadeOut()
{
    if (m_isFadingOut || BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_STOPPED || BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PAUSED) return;
    if (BASS_ChannelSlideAttribute(m_stream, BASS_ATTRIB_VOL, 0, m_fadeOutTime))
    {
        BASS_ChannelSetSync(m_stream, BASS_SYNC_SLIDE | BASS_SYNC_ONETIME, 0, &stopSyncProc, this);
        m_isFadingOut = true;
        m_state = FadingState;
        emit stateChanged();
    }
}

void RPlayer::fadeIn()
{
    if (BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING) return;
    BASS_ChannelSetAttribute(m_stream, BASS_ATTRIB_VOL, 0);
    play();
    BASS_ChannelSlideAttribute(m_stream, BASS_ATTRIB_VOL, m_volume, m_fadeInTime);
}

void RPlayer::playStop()
{
    BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? stop() : play();
}

void RPlayer::playPause()
{
    BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? pause() : play();
}

void RPlayer::playFadeOut()
{
    BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? fadeOut() : play();
}

void RPlayer::fadeInStop()
{
    BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? stop() : fadeIn();
}

void RPlayer::fadeInPause()
{
    if (BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PAUSED) play();
    else BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? pause() : fadeIn();
}

void RPlayer::fadeInFadeOut()
{
    BASS_ChannelIsActive(m_stream) == BASS_ACTIVE_PLAYING ? fadeOut() : fadeIn();
}

void RPlayer::timerEvent(QTimerEvent *)
{
    emit position(qint64(BASS_ChannelBytes2Seconds(m_stream, BASS_ChannelGetPosition(m_stream, BASS_POS_BYTE))*1000));
    if (!BASS_ChannelIsActive(m_stream)==BASS_ACTIVE_PLAYING) m_timer.stop();
}
