#
#    Copyright (C) 2011, Steffen Müller and the r.a.c.k. team.
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

include(../../libs/extensionsystem/plugins.pri)

QT += widgets
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += ../../shared

HEADERS = \
    ../../shared/rplayer.h \
    ../../shared/rbutton.h \
    ../../shared/rblinkbutton.h \
    ../../shared/rpushbutton.h \
    rpageedit.h \
    rindexbutton.h \
    rhotkeywidget.h \
    rhotkeywidgetplugin.h \
    ../../shared/rack.h

SOURCES = \
    ../../shared/rplayer.cpp \
    ../../shared/rbutton.cpp \
    ../../shared/rblinkbutton.cpp \
    ../../shared/rpushbutton.cpp \
    rpageedit.cpp \
    rindexbutton.cpp \
    rhotkeywidget.cpp \
    rhotkeywidgetplugin.cpp

TARGET          = $$qtLibraryTarget(rhotkeywidgetplugin)

# bass library:
unix:!macx: LIBS += -L$$PWD/../../libs/3rdparty/bass/x64/ -lbass
LIBS += -L$$PWD/../../libs/3rdparty/bass/ -lbass
INCLUDEPATH += $$PWD/../../libs/3rdparty/bass
DEPENDPATH += $$PWD/../../libs/3rdparty/bass













