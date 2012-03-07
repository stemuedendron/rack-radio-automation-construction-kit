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

#include "coreimpl.h"
#include "rackwindow.h"

#include <QtGui>


CoreImpl::CoreImpl(RackWindow *mainwindow) :
    m_mainwindow(mainwindow),
    m_state(ICore::NormalState),
    m_oldState(ICore::NormalState),
    m_fileSystemModel(new QFileSystemModel(this))
{

//    qRegisterMetaType<ICore::CoreState>("CoreState");

    //create models:
    QStringList filters;
    filters << "*.wav" << "*.ogg" << "*.aiff" << "*.mp3" << "*.mp2" << "*.mp1" << "*.mus";
    m_fileSystemModel->setRootPath("/");
    m_fileSystemModel->setNameFilters(filters);
    m_fileSystemModel->setNameFilterDisables(false);
    m_fileSystemModel->sort(Qt::AscendingOrder);

    m_modelList.append(m_fileSystemModel);

    startTimer(1000);
}


ICore::CoreState CoreImpl::state() const
{
    return m_state;
}

void CoreImpl::setNormalState()
{
    if (m_state == ICore::NormalState) return;
    toggleState(ICore::NormalState, ICore::NormalState);
}

void CoreImpl::toggleInsertState()
{
    if (m_state == ICore::NormalState or m_state == ICore::InsertState) toggleState(ICore::InsertState, ICore::NormalState);
}

void CoreImpl::toggleDeleteState()
{
    toggleState(ICore::DeleteState, ICore::NormalState);
}

void CoreImpl::togglePreviewState()
{
    toggleState(ICore::PreviewState, ICore::NormalState);
}

void CoreImpl::toggleState(ICore::CoreState stateOne, ICore::CoreState stateTwo)
{
    m_oldState = m_state;
    m_state == stateOne ? m_state = stateTwo : m_state = stateOne;
    emit stateChanged(m_state);
    emitStateChangeSignals(m_oldState, false);
    emitStateChangeSignals(m_state, true);
}

void CoreImpl::emitStateChangeSignals(ICore::CoreState state, bool set)
{
    switch (state)
    {
        case ICore::NormalState:
            emit normalStateChanged(set);
            break;
        case ICore::InsertState:
            emit insertStateChanged(set);
            break;
        case ICore::PreviewState:
            emit previewStateChanged(set);
            break;
        case ICore::DeleteState:
            emit deleteStateChanged(set);
            break;
    }
}


QList<QAbstractItemModel *> CoreImpl::modelList() const
{
    return m_modelList;
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






