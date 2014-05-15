/*
    Copyright (C) 2012, Steffen Müller.
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

#include "rlibrarybutton.h"
#include <QtWidgets>

RLibraryButton::RLibraryButton(QWidget *parent) :
    RPushButton(parent),
    m_title(new QLabel),
    m_artist(new QLabel),
    m_time(new QLabel),
    m_id(new QLabel)
{
    setObjectName("rackLibraryViewButton");
    m_title->setObjectName("rackLibraryViewButtonTitle");
    m_artist->setObjectName("rackLibraryViewButtonLabel");
    m_time->setObjectName("rackLibraryViewButtonLabel");
    m_id->setObjectName("rackLibraryViewButtonId");

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(m_title, 0, 0, 1, 2);
    gl->addWidget(m_artist, 1, 0, 1, 2);
    gl->addWidget(m_time, 2, 0, 1, 1);
    gl->addWidget(m_id, 2, 1, 1, 1, Qt::AlignRight);
    setLayout(gl);
}

void RLibraryButton::setTitle(const QString &title)
{
    if (m_title->text() == title) return;
    m_title->setText(title);
}

void RLibraryButton::setArtist(const QString &artist)
{
    if (m_artist->text() == artist) return;
    m_artist->setText(artist);
}

void RLibraryButton::setTime(const QString &time)
{
    if (m_time->text() == time) return;
    m_time->setText(time);
}

void RLibraryButton::setId(const QString &id)
{
    if (m_id->text() == id) return;
    m_id->setText(id);
}

