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

#include "rselectplugindialog.h"
#include "rpushbutton.h"

#include <QtGui>

RSelectPluginDialog::RSelectPluginDialog(QWidget *parent) :
    QDialog(parent),
    m_index(-1)
{

            //create header widget:
//            QWidget *header = new QWidget;
//            header->setObjectName("rackWidgetHeader");
//            QLabel *title = new QLabel(tr("Hotkeys"));
//            title->setObjectName("rackWidgetHeaderTitle");
//            m_pageTitle = new QLabel;
//            m_pageTitle->setObjectName("rackWidgetHeaderSubTitle");





    //ok connection
    //connect(bt, SIGNAL(pressed()),this, SLOT(setIndex()));

//    RPushButton *closebt = new RPushButton;

//    closebt->setFixedSize(60,60);

//    connect(closebt,SIGNAL(pressed()),SLOT(close()));

    //setMask(geometry());
}

void RSelectPluginDialog::setIndex()
{
    //RPushButton *senderButton = qobject_cast<RPushButton *>(sender());
    //colorValue = senderButton->getColor();


    done(true);
}

int RSelectPluginDialog::getIndex(QWidget *parent, const QStringList &plugins, bool *ok)
{
    RSelectPluginDialog dialog(parent);
    int ret = dialog.exec();
    if (ok) *ok = !!ret;
    if (ret) return dialog.m_index;
}
