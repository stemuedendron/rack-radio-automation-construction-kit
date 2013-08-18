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

#include "mainwindow.h"
#include "rcolordialog.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    bt = new QPushButton(this);
    bt->resize(200,100);

    connect(bt, SIGNAL(pressed()),SLOT(setColor()));

}

void MainWindow::setColor()
{
    bool ok;
    QColor color = RColorDialog::getColor(this, tr("Choose a color"), &ok);
    if (ok)
        ;
        //change this!! use style sheet name and set color indirect
        //bt->setColorButton(color);
}
