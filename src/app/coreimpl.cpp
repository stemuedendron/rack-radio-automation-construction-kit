/*
    Copyright (C) 2011, Steffen Müller.
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

#include "coreimpl.h"
#include "rackwindow.h"
#include "rcouchdbmodel.h"

#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QNetworkProxyFactory>
#include <QtSql>


CoreImpl::CoreImpl(RackWindow *mainwindow) :
    m_mainwindow(mainwindow),
    m_state(Rack::NormalState),
    m_networkAccessManager(new QNetworkAccessManager(this)),
    m_oldState(Rack::NormalState),
    m_fileSystemModel(new QFileSystemModel(this)),
    m_sqlQueryModel(new QSqlQueryModel(this))
{

    //models:
    //filesystem:
    QStringList filters;
    filters << "*.wav" << "*.ogg" << "*.aiff" << "*.mp3" << "*.mp2" << "*.mp1" << "*.mus";
    m_fileSystemModel->setRootPath("/");
    m_fileSystemModel->setNameFilters(filters);
    m_fileSystemModel->setNameFilterDisables(false);
    m_fileSystemModel->sort(Qt::AscendingOrder);

    m_modelList.append(m_fileSystemModel);

    //sql
    m_sqlQueryModel->setQuery("SELECT * FROM audio ORDER BY artist");

    m_modelList.append(m_sqlQueryModel);

    //couchdb
    //localhost:5984/rackdb/_design/library/_view/media
    QUrl url("http://localhost:5984/rackdb/_design/library/_view/media");
    RCouchDBModel *couchDBModel = new RCouchDBModel(m_networkAccessManager, url, this);
    m_modelList.append(couchDBModel);


    startTimer(1000);
}


Rack::CoreState CoreImpl::state() const
{
    return m_state;
}

QNetworkAccessManager *CoreImpl::networkAccessManager() const
{
    return m_networkAccessManager;
}

QList<QAbstractItemModel *> CoreImpl::modelList() const
{
    return m_modelList;
}

void CoreImpl::setNormalState()
{
    if (m_state == Rack::NormalState) return;
    toggleState(Rack::NormalState, Rack::NormalState);
}

void CoreImpl::toggleInsertState()
{
    if (m_state == Rack::NormalState or m_state == Rack::InsertState)
    {
        toggleState(Rack::InsertState, Rack::NormalState);
    }
}

void CoreImpl::toggleDeleteState()
{
    toggleState(Rack::DeleteState, Rack::NormalState);
}

void CoreImpl::togglePreviewState()
{
    toggleState(Rack::PreviewState, Rack::NormalState);
}

void CoreImpl::toggleState(Rack::CoreState stateOne, Rack::CoreState stateTwo)
{
    m_oldState = m_state;
    m_state == stateOne ? m_state = stateTwo : m_state = stateOne;
    emit stateChanged(m_state);
    emitStateChangeSignals(m_oldState, false);
    emitStateChangeSignals(m_state, true);
}

void CoreImpl::emitStateChangeSignals(Rack::CoreState state, bool set)
{
    switch (state)
    {
        case Rack::NormalState:
            emit normalStateChanged(set);
            break;
        case Rack::InsertState:
            emit insertStateChanged(set);
            break;
        case Rack::PreviewState:
            emit previewStateChanged(set);
            break;
        case Rack::DeleteState:
            emit deleteStateChanged(set);
            break;
    }
}

void CoreImpl::timerEvent(QTimerEvent *)
{
    emit timeStrChanged(QTime::currentTime().toString(tr("hh:mm:ss")));
    emit dateStrChanged(QDate::currentDate().toString(tr("dd.MM.yy")));
}

void CoreImpl::getHello(const QString &str)
{
    m_mainwindow->setWindowTitle(str);
}

CoreImpl::~CoreImpl()
{

}






