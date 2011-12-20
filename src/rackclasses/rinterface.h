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

#ifndef RINTERFACE_H
#define RINTERFACE_H

#include "rack.h"
#include <QWidget>

class RInterface
{
public:
    virtual ~RInterface() {}

    virtual QWidget *createRWidget(QWidget *parent, Rack *api) = 0;

//in this way we can plugins force to implement api methods:
//public slots:

//    virtual void setDate(const QString &) = 0;
//    virtual void setTime(const QString &) = 0;

//signals:

//    virtual void sayHello(const QString &) = 0;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(RInterface, "org.rack.Rack.RInterface/1.0")
QT_END_NAMESPACE


#endif
