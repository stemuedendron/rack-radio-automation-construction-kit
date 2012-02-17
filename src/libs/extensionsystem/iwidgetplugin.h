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

#ifndef IWIDGETPLUGIN_H
#define IWIDGETPLUGIN_H

#include "icore.h"
#include <QWidget>

class IWidgetPlugin
{
public:
    virtual ~IWidgetPlugin() {}

    virtual QWidget *createRWidget(ICore *api, QWidget *parent = 0) = 0;

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString version() const = 0;
    virtual QString url() const = 0;
    virtual QString vendor() const = 0;
    virtual QString license() const = 0;
    virtual QString copyright() const = 0;

//in this way we can plugins force to implement api methods:
//public slots:

//    virtual void setDate(const QString &) = 0;
//    virtual void setTime(const QString &) = 0;

//signals:

//    virtual void sayHello(const QString &) = 0;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IWidgetPlugin, "org.rack.IWidgetPlugin/1.0")
QT_END_NAMESPACE


#endif //IWIDGETPLUGIN_H
