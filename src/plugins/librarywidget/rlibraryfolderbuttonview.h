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

#ifndef RLIBRARYFOLDERBUTTONVIEW_H
#define RLIBRARYFOLDERBUTTONVIEW_H

#include <QWidget>
#include <QPersistentModelIndex>

class ICore;
class QAbstractItemModel;
class QModelIndex;
class RPushButton;
class RLibraryButton;

class RLibraryFolderButtonView : public QWidget
{
    Q_OBJECT

public:

    explicit RLibraryFolderButtonView(ICore *api, QWidget *parent = 0);

    void setModel (QAbstractItemModel *model);
    QAbstractItemModel *model() const;

    void setRootIndex(const QModelIndex &index);
    QModelIndex rootIndex() const;

public slots:

    void reset();

protected slots:

    void modelDestroyed();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void rowsRemoved(const QModelIndex&, int, int);
    void layoutChanged();

    void setButtonData();
    void buttonClicked();
    void updateButtonCount();
    void oneLevelUp();
    void previousPage();
    void nextPage();

signals:

    void clicked(const QModelIndex &index);

protected:

    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *event);

private:

    ICore *m_core;
    QAbstractItemModel *m_model;
    QPersistentModelIndex m_root;
    QWidget *m_header;
    QWidget *m_buttons;
    QWidget *m_footer;

    //we need to store row and column number because QGridLayout only increase the numbers
    int m_rows;
    int m_cols;
    int m_currentPage;

    RLibraryButton *createOneButton();

};

#endif // RLIBRARYFOLDERBUTTONVIEW_H
