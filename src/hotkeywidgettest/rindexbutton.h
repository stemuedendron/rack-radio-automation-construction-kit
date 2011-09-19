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

#ifndef RINDEXBUTTON_H
#define RINDEXBUTTON_H

#include <QPushButton>

class QLabel;

class RIndexButton : public QPushButton
{
    Q_OBJECT

public:

    explicit RIndexButton(const QString &title, const int &keys, QWidget *parent = 0);
    void setTitle(const QString &title);
    void setKeys(const int &keys);
    QString title() const;

public slots:

    void setEditMode(const bool &);

private:

    QLabel *m_title;
    QLabel *m_keys;
    QLabel *m_edit;

};

#endif // RINDEXBUTTON_H
