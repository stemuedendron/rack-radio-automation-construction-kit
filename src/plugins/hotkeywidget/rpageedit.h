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

#ifndef RPAGEEDIT_H
#define RPAGEEDIT_H

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QTabWidget;
class QLabel;

class RPageEdit : public QDialog
{

    Q_OBJECT

public:

    explicit RPageEdit(QWidget *parent = 0);
    QLineEdit *leEditTitle;
    QLineEdit *leAddTitle;
    QLineEdit *leCloneTitle;
    QSpinBox *sbEditRows;
    QSpinBox *sbEditCols;
    QSpinBox *sbAddRows;
    QSpinBox *sbAddCols;
    QTabWidget *tabWidget;

private slots:

    void okClicked();

private:

    QLabel *m_editEmpty;

};

#endif // RPAGEEDIT_H
