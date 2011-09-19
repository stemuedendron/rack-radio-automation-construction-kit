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

TEMPLATE = app
TARGET = hotkeywidgettest

HEADERS += mainwindow.h \  
    rhotkeys.h \
    rpageedit.h \
    rindexbutton.h \
    ../rackclasses/rbutton.h \
    ../rackclasses/rblinkbutton.h \
    ../rackclasses/rplayer.h

SOURCES += main.cpp \ 
    mainwindow.cpp \
    rhotkeys.cpp \
    rpageedit.cpp \
    rindexbutton.cpp \
    ../rackclasses/rbutton.cpp \
    ../rackclasses/rblinkbutton.cpp \
    ../rackclasses/rplayer.cpp

RESOURCES += \
    ../rack.qrc

OTHER_FILES += \
    ../../qss/default.qss \
    ../../qss/matrix.qss \
    ../../qss/default-old.qss

INCLUDEPATH += ../rackclasses


# bass library:
unix:!macx: LIBS += -L$$PWD/../../bass/x64/ -lbass
LIBS += -L$$PWD/../../bass/ -lbass
INCLUDEPATH += $$PWD/../../bass
DEPENDPATH += $$PWD/../../bass




