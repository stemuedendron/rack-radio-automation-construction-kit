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

SOURCES += \
    main.cpp \
    coreimpl.cpp \
    rackwindow.cpp \
    rsplitter.cpp \
    rselectplugindialog.cpp \
    ../shared/rpushbutton.cpp \
    ../shared/rblinkbutton.cpp

HEADERS += \
    coreimpl.h \
    icore.h \
    rackwindow.h \
    rsplitter.h \
    rselectplugindialog.h \
    ../libs/extensionsystem/iwidgetplugin.h \
    ../shared/rpushbutton.h \
    ../shared/rblinkbutton.h

INCLUDEPATH += \
    ../libs/extensionsystem \
    ../shared/

RESOURCES += \
    ../resources/rack.qrc

OTHER_FILES += \
    ../resources/stylesheets/default.qss








































