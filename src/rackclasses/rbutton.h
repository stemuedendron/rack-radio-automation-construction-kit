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

#ifndef RBUTTON_H
#define RBUTTON_H

#include "rplayer.h"
#include <QPushButton>


class QColor;

class RButton : public QPushButton
{
    Q_OBJECT
    Q_ENUMS(ButtonKind)

    Q_PROPERTY(ButtonKind bk READ bk)

    Q_PROPERTY(QColor positionClockColor READ positionClockColor WRITE setPositionClockColor)

    //we need this property to sync the position clock corners with the button corners
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)

    Q_PROPERTY(RPlayer::State playerState READ playerState)

public:

    enum ButtonKind
    {
        bkGrad,
        bkOnAir,
        bkStack,
        bkHotKey,
        bkLibrary
    };

    explicit RButton(ButtonKind bk = bkGrad, QWidget *parent = 0);
    ~RButton();

    ButtonKind bk() const {return m_bk;}

    QColor positionClockColor() const {return m_positionClockColor;}
    int borderRadius() const {return m_borderRadius;}
    RPlayer::State playerState() const {return m_player->getState();}

signals:


public slots:

    void setPositionClockColor(const QColor &);
    void setBorderRadius(const int &);

private slots:
    void setLength(const qint64 &);
    void setPosition(const qint64 &);

protected:

    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);

private:

    void drawClock(QPainter *);
    void refreshImage();

    ButtonKind m_bk;

    RPlayer *m_player;
    int m_device;

    qint64 m_position;
    QColor m_positionClockColor;
    int m_borderRadius;
    qint64 m_length;
    int m_drawPosition;

};

#endif // RBUTTON_H
