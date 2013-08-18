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

#ifndef RPLAYER_H
#define RPLAYER_H

#include "bass.h"
#include <QObject>
#include <QBasicTimer>

struct audiodevice
{
    int engineIndex;
    bool isInitialized;
    QString name;
    QString driver;
    QString userComment;
};

class RPlayer : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QString uri READ uri WRITE setURI)

public:

    enum State
    {
        LoadingState,
        PlayingState,
        PausedState,
        FadingState,
        StoppedState,
        ErrorState,
        DisabledState
    };

    /**
     * @brief Create RPlayer object with the given device.
     *        You have to call RPlayer::init(int device) first.
     *        You can get a list of devices with RPlayer::getDevices()
     * @param parent
     */
    explicit RPlayer(QObject *parent = 0, int device = 0);

    ~RPlayer();

    State getState() const {return m_state;}

    static bool init(int device);
    static bool free(int device);
    static QList<audiodevice> getDevices();

    QString uri() const {return m_uri;}
    void setURI(const QString &uri) {m_uri = uri;}


public slots:

    void setDevice(int);
    void setVolume(float);
    void setFadeInTime(DWORD);
    void setFadeOutTime(DWORD);
//    void setURI(const QString &);
    void play();
    void pause();
    void stop();
    void fadeOut();
    void fadeIn();
    void playPause();
    void playStop();
    void playFadeOut();
    void fadeInStop();
    void fadeInPause();
    void fadeInFadeOut();

signals:

    /**
     * @brief This signal emmits the current position (in milliseconds) of the file currently being played.
     *
     * @param int time
     */
    void position(qint64 position);

    /**
     * @brief This signal emmits the total length (in milliseconds) of the file currently loaded.
     *        Must be connected before you call setURI.
     * @param int
     */
    void time(qint64 time);

    /**
     * @brief This signal is emitted when the player state has changed.
     *
     * @param
     */
    void stateChanged();

protected:

    void timerEvent(QTimerEvent *event);

private:

    int m_device;
    State m_state;
    HSTREAM m_stream; /**< TODO */
    float m_volume; /**< TODO */
    DWORD m_fadeInTime; /**< TODO */
    DWORD m_fadeOutTime; /**< TODO */
    bool m_isFadingOut; /**< TODO */
    QBasicTimer m_timer; /**< TODO */

    QString m_uri;



};

#endif // RPLAYER_H
