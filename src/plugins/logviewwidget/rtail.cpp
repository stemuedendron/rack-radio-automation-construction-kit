/*
    Copyright (C) 2015, Steffen Müller.
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

#include "rtail.h"


RTail::RTail(const QString &fileName, QObject *parent) :
    QProcess(parent)
{
    QStringList argv;
    argv << "-f" << fileName;
    start("tail", argv);
    connect (this, SIGNAL(readyReadStandardOutput()), this, SLOT(logOutput()));
}

RTail::~RTail()
{
    terminate();
}

void RTail::logOutput()
{
    QByteArray bytes = readAllStandardOutput();
    QStringList lines = QString(bytes).split("\n");
    foreach (QString line, lines)
    {
        emit logString(line);
    }
}
