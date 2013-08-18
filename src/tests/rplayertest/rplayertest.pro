#
#    Copyright (C) 2011, Steffen Müller.
#    All rights reserved.
#
#    This file is part of r.a.c.k. radio automation construction kit
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Author: Steffen Müller
#

QT += widgets

TEMPLATE = app
TARGET = rplayertest

DEPENDPATH += .

INCLUDEPATH += ../../shared

HEADERS += mainwindow.h \ 
    ../../shared/rplayer.h

SOURCES += main.cpp \ 
    mainwindow.cpp \
    ../../shared/rplayer.cpp

# bass library:
unix:!macx: LIBS += -L$$PWD/../../libs/3rdparty/bass/x64/ -lbass
LIBS += -L$$PWD/../../libs/3rdparty/bass/ -lbass
INCLUDEPATH += $$PWD/../../libs/3rdparty/bass
DEPENDPATH += $$PWD/../../libs/3rdparty/bass
