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

#ifndef ICORE_H
#define ICORE_H

#include <QObject>

class QAbstractItemModel;
class QItemSelection;

//this is the rack core api interface
class ICore : public QObject
{
    Q_OBJECT

public:

    ICore() {}
    virtual ~ICore() {}

    enum State
    {
        NormalState,
        InsertState,
        PreviewState,
        DeleteState
    };

    virtual State state() const = 0;

    virtual QList<QAbstractItemModel *> modelList() const = 0;


    //plugins store there selection(s) here and have to make sure
    //the selection is properly cleared

    //if we only want one item to be inserted just use a QModelIndex
    //QItemSelection currentItemSelection;

public slots:

    virtual void setNormalState() = 0;
    virtual void setInsertState(bool set) = 0;
    virtual void setPreviewState(bool set) = 0;

    virtual void getHello(const QString &) = 0;

signals:

    void normalStateChanged(bool set);
    void insertStateChanged(bool set);
    void previewStateChanged(bool set);
    void deleteStateChanged(bool set);

    void timeStrChanged(QString);
    void dateStrChanged(QString);

};

#endif // ICORE_H
