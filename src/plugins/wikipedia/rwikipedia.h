/*
    Copyright (C) 2013, Steffen Müller.
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

#ifndef RWIKIPEDIA_H
#define RWIKIPEDIA_H

#include <QWidget>

class ICore;

class RWikipedia : public QWidget
{
    Q_OBJECT

public:

    explicit RWikipedia(ICore *api, QWidget *parent = 0);

private:

    ICore* m_core;

};


#endif // RWIKIPEDIA_H
