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

TEMPLATE = lib
CONFIG += plugin

HEADERS = \
    rlibrarywidget.h \
    rlibrarywidgetplugin.h \
    ../../shared/rpushbutton.h \
    rlibraryfolderbuttonview.h \
    rlibrarybutton.h \
    ../../shared/rack.h

SOURCES = \
    rlibrarywidget.cpp \
    rlibrarywidgetplugin.cpp \
    ../../shared/rpushbutton.cpp \
    rlibraryfolderbuttonview.cpp \
    rlibrarybutton.cpp

TARGET = $$qtLibraryTarget(librarywidgetplugin)


