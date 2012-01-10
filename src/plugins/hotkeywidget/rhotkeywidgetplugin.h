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

#ifndef RHOTKEYWIDGETPLUGIN_H
#define RHOTKEYWIDGETPLUGIN_H

#include "iwidgetplugin.h"
#include <QObject>


class RHotKeyWidgetPlugin : public QObject, IWidgetPlugin
{
    Q_OBJECT
    Q_INTERFACES(IWidgetPlugin)

public:

    QWidget *createRWidget(QWidget *parent, ICore *api);

    QString name() const { return "Hotkeys"; }
    QString description() const { return tr("The Hotkeys widget let you instantly play jingles, trailer, music beds or whatever you want."); }
    QString version() const { return "0.0.1"; }
    QString url() const { return "http://rack-radio-automation-construction-kit.googlecode.com/"; }
    QString vendor() const { return "rack"; }
    QString license() const { return "GNU GPLv3+"; }
    QString copyright() const { return "Copyright (C) Steffen Müller"; }

};


#endif //RHOTKEYWIDGETPLUGIN_H
