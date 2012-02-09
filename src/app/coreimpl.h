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

#ifndef COREIMPL_H
#define COREIMPL_H

#include "icore.h"

class RackWindow;
class QFileSystemModel;

class CoreImpl : public ICore
{
    Q_OBJECT

public:

    CoreImpl(RackWindow *mainwindow);
    ~CoreImpl();

    QList<QAbstractItemModel *> modelList() const;

public slots:

    void getHello(const QString &);

private:

    RackWindow *m_mainwindow;

    QList<QAbstractItemModel *> m_modelList;

    //core implemented models to be used in plugin widgets
    QFileSystemModel *m_fileSystemModel;

    //maybe we need this?
    //friend class RackWindow;


protected:

     void timerEvent(QTimerEvent *);

};

#endif // COREIMPL_H
