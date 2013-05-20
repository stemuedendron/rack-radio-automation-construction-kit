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

#include "rindexbutton.h"
#include <QtWidgets>

RIndexButton::RIndexButton(const QString &title, int keys, QWidget *parent) :
    RPushButton(parent)
{
    setObjectName("rackHotkeyIndexButton");
    m_title = new QLabel;
    m_title->setObjectName("rackHotkeyIndexButtonTitleLabel");
    m_keys = new QLabel;
    m_keys->setObjectName("rackHotkeyIndexButtonKeysLabel");
    setTitle(title);
    setKeys(keys);

    m_edit = new QLabel(tr("Edit Mode"));
    m_edit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_edit->setObjectName("rackHotkeyEditMode");
    m_edit->setHidden(true);
    QVBoxLayout * vl = new QVBoxLayout;
    vl->addWidget(m_edit, 0, Qt::AlignJustify | Qt::AlignTop);
    QWidget *lbwidget = new QWidget(this);
    lbwidget->setLayout(vl);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(m_title);
    hl->addWidget(m_keys);
    hl->addStretch();
    QWidget *widget = new QWidget(this);
    widget->setLayout(hl);

    QStackedLayout *sl = new QStackedLayout;
    sl->setStackingMode(QStackedLayout::StackAll);
    sl->addWidget(widget);
    sl->addWidget(lbwidget);
    setLayout(sl);
}

void RIndexButton::setTitle(const QString &title)
{
    if (m_title->text() == title) return;
    m_title->setText(title);
}

void RIndexButton::setKeys(int keys)
{
    QString keysStr = "(" + QString::number(keys) + " " + tr("keys") + ")";
    if (m_keys->text() == keysStr) return;
    m_keys->setText(keysStr);
}

QString RIndexButton::title() const
{
    return m_title->text();
}

void RIndexButton::setEditMode(bool edit)
{
    m_edit->setVisible(edit);
}
