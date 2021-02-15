/*
    Copyright (C) 2012, Steffen Müller.
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

#include "icore.h"
#include "rlibrarywidget.h"
#include "rlibraryfolderbuttonview.h"

#include <QtWidgets>

RLibraryWidget::RLibraryWidget(ICore *api, QWidget *parent) :
    QWidget(parent),
    m_core(api),
    m_model(0),
    m_folderButtonView(new RLibraryFolderButtonView(m_core, this))
{

    //TODO: hardcoded model select:
    if (!m_core->modelList().isEmpty())
    {
        m_model = m_core->modelList().at(2);
    }

    //proxy model for searching:
    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(m_model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterKeyColumn(-1);

    m_folderButtonView->setModel(proxy);

//    QFileSystemModel *model = qobject_cast<QFileSystemModel *>(m_model);
//    if (model)
//    {
//        m_folderButtonView->setRootIndex(model->index(model->rootPath()));
//    }

//    QObject::connect(m_folderButtonView, SIGNAL(clicked(QModelIndex)), this, SLOT(listClicked(QModelIndex)));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_folderButtonView);
    setLayout(layout);

    connect(m_folderButtonView, SIGNAL(textChanged(QString)), proxy, SLOT(setFilterFixedString(QString)));

}

void RLibraryWidget::listClicked(const QModelIndex &index)
{
    if (m_model->hasChildren(index))
    {
        m_folderButtonView->setRootIndex(index);
    }
}
