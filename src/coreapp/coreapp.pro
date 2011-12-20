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

TARGET = rack

SOURCES += main.cpp \
    rackimpl.cpp \
    rdesktop.cpp \
    rpluginhost.cpp \
    ../rackclasses/rsplitter.cpp

HEADERS += rackimpl.h \
    rdesktop.h \
    rpluginhost.h \
    ../rackclasses/rack.h \
    ../rackclasses/rinterface.h \
    ../rackclasses/rsplitter.h

INCLUDEPATH += ../rackclasses

RESOURCES += \
    ../rack.qrc

OTHER_FILES += \
    ../../qss/default.qss

# bass library:
unix:!macx: LIBS += -L$$PWD/../../bass/x64/ -lbass
LIBS += -L$$PWD/../../bass/ -lbass
INCLUDEPATH += $$PWD/.././bass
DEPENDPATH += $$PWD/../../bass




























