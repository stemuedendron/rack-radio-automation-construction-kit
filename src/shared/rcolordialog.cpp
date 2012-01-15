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

#include "rcolordialog.h"
#include "rpushbutton.h"

#include <QtGui>


RColorDialog::RColorDialog(QWidget *parent) :
    QDialog(parent)
{

    setFixedSize(220,300);
    QGridLayout *l = new QGridLayout;
    l->setSpacing(3);
    QList<QColor> colorlist;
    colorlist << "#1D48BB" << "#06829E" << "#006600" << "#990000" << "#800080" << "#FFAA00" << "#A6A6A6"<<"#505050" <<"#000000";
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            RPushButton *bt = new RPushButton;
            bt->setFixedSize(60,60);

            connect(bt, SIGNAL(pressed()),SLOT(setColor()));
            l->addWidget(bt, i, j);
        }
    RPushButton *closebt = new RPushButton;

    closebt->setFixedSize(60,60);

    connect(closebt,SIGNAL(pressed()),SLOT(close()));
    l->addWidget(closebt,3,2);
    setLayout(l);


    setMask(geometry());


}


void RColorDialog::setColor()
{
    RPushButton *senderButton = qobject_cast<RPushButton *>(sender());
    //colorValue = senderButton->getColor();
    done(true);
}

QColor RColorDialog::getColor(QWidget *parent, const QString &title, bool *ok)
{
    RColorDialog dialog(parent);
    dialog.setWindowTitle(title);
    int ret = dialog.exec();
    if (ok) *ok = !!ret;
    if (ret) return dialog.colorValue;
}
