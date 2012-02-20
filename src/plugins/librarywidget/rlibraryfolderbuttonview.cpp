/*
    Copyright (C) 2012, Steffen Müller and the r.a.c.k. team.
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

#include "rlibraryfolderbuttonview.h"
#include "icore.h"
#include "rpushbutton.h"
#include "rlibrarybutton.h"
#include <QtGui>


/* BUG: we get a crash if we:
   - load library widegt plugin
   - click insert normal state test button
   - delete plugin
   - load it again and click state change button
*/


RLibraryFolderButtonView::RLibraryFolderButtonView(ICore *api, QWidget *parent) :
    QWidget(parent),
    m_core(api),
    m_model(0),
    m_header(new QWidget),
    m_buttons(new QWidget),
    m_footer(new QWidget),
    m_rows(0),
    m_cols(0),
    m_currentPage(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //header
    m_header->setObjectName("rackWidgetHeader");

    QLabel *title = new QLabel(tr("Library"));
    title->setObjectName("rackWidgetHeaderTitle");
    QLabel *subTitle = new QLabel(tr("File System"));
    subTitle->setObjectName("rackWidgetHeaderSubTitle");

    RPushButton *b0 = new RPushButton;
    b0->setObjectName("rackTopArrowButton");
    b0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    RPushButton *b1 = new RPushButton;
    b1->setObjectName("rackLeftArrowButton");
    b1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    b1->setAutoRepeat(true);

    RPushButton *b2 = new RPushButton;
    b2->setObjectName("rackRightArrowButton");
    b2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    b2->setAutoRepeat(true);

    QGridLayout *headerLayout = new QGridLayout(m_header);
    headerLayout->addWidget(title,    0, 0);
    headerLayout->addWidget(subTitle, 1, 0);
    headerLayout->addWidget(b0,       0, 1, 2, 1);
    headerLayout->addWidget(b1,       0, 2, 2, 1);
    headerLayout->addWidget(b2,       0, 3, 2, 1);

    //footer
    m_footer->setObjectName("rackWidgetFooter");

    RPushButton *b3 = new RPushButton(tr("New"));
    b3->setObjectName("rackFooterButton");


    //state test:
    m_core->normalState->addTransition(b3, SIGNAL(clicked()), m_core->insertState);
    m_core->insertState->addTransition(b3, SIGNAL(clicked()), m_core->normalState);
//    m_core->normalState->assignProperty(b3, "text", "normalState");
//    m_core->insertState->assignProperty(b3, "text", "insertState");
//    m_core->deleteState->assignProperty(b3, "text", "delete");
//    m_core->previewState->assignProperty(b3, "text", "preview");





    RPushButton *b4 = new RPushButton(tr("Info"));
    b4->setObjectName("rackFooterButton");
    RPushButton *b5 = new RPushButton(tr("Edit"));
    b5->setObjectName("rackFooterButton");
    RPushButton *b6 = new RPushButton(tr("Delete"));
    b6->setObjectName("rackFooterButton");

    QHBoxLayout *footerLayout = new QHBoxLayout(m_footer);
    footerLayout->addStretch();
    footerLayout->addWidget(b3);
    footerLayout->addWidget(b4);
    footerLayout->addWidget(b5);
    footerLayout->addWidget(b6);
    footerLayout->addStretch();

    //buttons
    new QGridLayout(m_buttons);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_header);
    layout->addWidget(m_buttons);
    layout->addWidget(m_footer);
    setLayout(layout);

    QObject::connect(b0, SIGNAL(clicked()), this, SLOT(oneLevelUp()));
    QObject::connect(b1, SIGNAL(clicked()), this, SLOT(previousPage()));
    QObject::connect(b2, SIGNAL(clicked()), this, SLOT(nextPage()));

}

void RLibraryFolderButtonView::resizeEvent(QResizeEvent *)
{
    QTimer::singleShot(100, this, SLOT(updateButtonCount()));
}

void RLibraryFolderButtonView::updateButtonCount()
{
    //TODO: change these to properties
    int newRows = m_buttons->height() / 80;
    int newCols = m_buttons->width() / 180;

    if (newRows == 0) newRows = 1;
    if (newCols == 0) newCols = 1;

    if (newRows == m_rows && newCols == m_cols) return;

    m_buttons->setUpdatesEnabled(false);
    qDeleteAll(m_buttons->findChildren<RLibraryButton *>());
    delete m_buttons->layout();
    m_buttons->setUpdatesEnabled(true);
    QGridLayout *newLayout = new QGridLayout(m_buttons);
    newLayout->setSpacing(3);
    newLayout->setContentsMargins(0,0,0,0);
    for (int row = 0; row < newRows; row++)
    {
        for (int col = 0; col < newCols; col++)
        {
            RLibraryButton *b = createOneButton();
            newLayout->addWidget(b, row, col);
        }
    }

    //calculate new page number
    m_currentPage = (m_currentPage * m_rows * m_cols) / m_buttons->layout()->count();

    qDebug() << "page: " << m_currentPage;

    //save new row and col number
    m_rows = newRows;
    m_cols = newCols;
    //refresh model data
    setButtonData();

}

RLibraryButton *RLibraryFolderButtonView::createOneButton()
{
    RLibraryButton *b = new RLibraryButton;
    b->setObjectName("rackLibraryViewButton");
    b->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    //we silently ignore user set style sheet geometry values by overriding them :
    b->setStyleSheet("#rackLibraryViewButton { min-height: -1px; min-width: -1px; height: -1px; width: -1px; max-height: -1px; max-width: -1px;}");
    b->setDisabled(true);

    QObject::connect(b, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    return b;
}

void RLibraryFolderButtonView::setButtonData()
{
    if (m_model->hasChildren(m_root))
    {
        if (m_model->canFetchMore(m_root))
        {
            m_model->fetchMore(m_root);
        }
    }

    int indexOffset = m_currentPage * m_buttons->layout()->count();

    for (int i = 0; i < m_buttons->layout()->count(); i++)
    {
        RLibraryButton *button = qobject_cast<RLibraryButton *>(m_buttons->layout()->itemAt(i)->widget());
        button->setProperty("isFolder", false);

        if (m_model->hasIndex(i + indexOffset, 0, m_root))
        {
            button->setText(m_model->data(m_model->index(i + indexOffset, 0, m_root)).toString());
            button->setEnabled(true);
            if (m_model->hasChildren(m_model->index(i + indexOffset, 0, m_root)))
            {
                button->setProperty("isFolder", true);
            }
        }
        else
        {
            button->setText("");
            button->setEnabled(false);
        }

        style()->unpolish(button);
        style()->polish(button);

    }
}

void RLibraryFolderButtonView::buttonClicked()
{
    if (!m_model) return;

    RLibraryButton *button = qobject_cast<RLibraryButton *>(sender());
    int buttonIndex = m_buttons->layout()->indexOf(button) + m_currentPage * m_buttons->layout()->count();

    if (m_model->hasIndex(buttonIndex, 0, m_root))
    {
        emit clicked(m_model->index(buttonIndex, 0, m_root));
    }
}

void RLibraryFolderButtonView::oneLevelUp()
{

//    QFileSystemModel *model = qobject_cast<QFileSystemModel *>(m_model);
//    if (model)
//    {
//        if (m_root == model->index(model->rootPath())) return;
//    }

    if (m_model->parent(m_root).isValid())
    {
        setRootIndex(m_model->parent(m_root));
    }
}

void RLibraryFolderButtonView::previousPage()
{
    //change this by disable button if we are on page 0 or have no model
    if (m_currentPage == 0) return;
    m_currentPage--;

    qDebug() << "page: " << m_currentPage;

    setButtonData();
}

void RLibraryFolderButtonView::nextPage()
{
    //needs checking if the model has more data
    m_currentPage++;

    qDebug() << "page: " << m_currentPage;

    setButtonData();
}

void RLibraryFolderButtonView::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        if (event->delta() < 0) nextPage();
        if (event->delta() > 0) previousPage();
    }
    event->accept();
}

void RLibraryFolderButtonView::setModel(QAbstractItemModel *model)
{

    if (model == m_model) return;

    if (m_model)
    {
        disconnect(m_model, SIGNAL(destroyed()), this, SLOT(modelDestroyed()));
        disconnect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
        disconnect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
        disconnect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
        disconnect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
        disconnect(m_model, SIGNAL(modelReset()), this, SLOT(reset()));
        disconnect(m_model, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));
    }

    m_model = (model ? model : 0);

    if (m_model)
    {
        connect(m_model, SIGNAL(destroyed()), this, SLOT(modelDestroyed()));
        connect(m_model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
        connect(m_model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
        connect(m_model, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)), this, SLOT(rowsAboutToBeRemoved(QModelIndex,int,int)));
        connect(m_model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemoved(QModelIndex,int,int)));
        connect(m_model, SIGNAL(modelReset()), this, SLOT(reset()));
        connect(m_model, SIGNAL(layoutChanged()), this, SLOT(layoutChanged()));
    }

}

QAbstractItemModel *RLibraryFolderButtonView::model() const
{
    return m_model;
}

void RLibraryFolderButtonView::setRootIndex(const QModelIndex &index)
{
    if (index.isValid() && index.model() != m_model) {
        qWarning("QAbstractItemView::setRootIndex failed : index must be from the currently set model");
        return;
    }
    m_root = index;
    m_currentPage = 0;
    setButtonData();
}

QModelIndex RLibraryFolderButtonView::rootIndex() const
{
    return QModelIndex(m_root);
}


void RLibraryFolderButtonView::modelDestroyed()
{
    qDebug() << "model: modelDestroyed";
}

void RLibraryFolderButtonView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    qDebug() << "model: dataChanged";
    setButtonData();
}

void RLibraryFolderButtonView::rowsInserted(const QModelIndex &parent, int start, int end)
{
    qDebug() << "model: rowsInserted";
//    setButtonData();
}

void RLibraryFolderButtonView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    qDebug() << "model: rowsAboutToBeRemoved";
    setButtonData();
}

void RLibraryFolderButtonView::rowsRemoved(const QModelIndex &, int, int)
{
    qDebug() << "model: rowsRemoved";
    setButtonData();
}

void RLibraryFolderButtonView::reset()
{
    qDebug() << "model: reset";
    qDebug() << "we have:" << m_model->rowCount();

    setButtonData();
}

void RLibraryFolderButtonView::layoutChanged()
{
    qDebug() << "model: layoutChanged";
    setButtonData();
}
