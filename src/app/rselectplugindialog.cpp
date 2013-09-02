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

#include "rselectplugindialog.h"
#include "rpushbutton.h"

#include <QtWidgets>

//dialog usage:
//    bool ok;
//    int newPluginIndex = RSelectPluginDialog::getIndex(this, StringList, &ok);
//    if (ok) ....

RSelectPluginDialog::RSelectPluginDialog(QWidget *parent) :
    QDialog(parent),
    pluginListWidget(new QListWidget),
    m_btOK(new QPushButton)
{


    //setObjectName("rackDialog");

    //create header widget:
    QWidget *header = new QWidget;
    header->setObjectName("rackWidgetHeader");
    QLabel *title = new QLabel(tr("Select Plugin"));
    title->setObjectName("rackWidgetHeaderTitle");
    QLabel *subTitle = new QLabel(tr("Currently the following plugins are available"));
    subTitle->setObjectName("rackWidgetHeaderSubTitle");

    QVBoxLayout *headerLayout = new QVBoxLayout(header);
    headerLayout->addWidget(title);
    headerLayout->addWidget(subTitle);

    //plugin list:
    pluginListWidget->setObjectName("rackSelectPluginList");

    //m_pluginList->addItems(plugins);
    //m_pluginList->setCurrentRow(0);

    //dialog buttons:
    m_btOK->setObjectName("rackOkButton");
    m_btOK->setEnabled(false);
    m_btOK->setDefault(true);

    RPushButton *btClose = new RPushButton;
    btClose->setObjectName("rackCloseButton");

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addStretch();
    hl->addWidget(m_btOK);
    hl->addWidget(btClose);



    //connections:
    connect(m_btOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btClose, SIGNAL(clicked()), this, SLOT(reject()));

    connect(pluginListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(currentItemChanged(QListWidgetItem*,QListWidgetItem*)));


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(header);
    layout->addWidget(pluginListWidget);
    layout->addLayout(hl);
    setLayout(layout);


    //setMask(geometry());


}

void RSelectPluginDialog::currentItemChanged(QListWidgetItem *, QListWidgetItem *)
{
    m_btOK->setEnabled(true);
}


//void RSelectPluginDialog::setIndex()
//{
//    //RPushButton *senderButton = qobject_cast<RPushButton *>(sender());
//    //colorValue = senderButton->getColor();


//    //change this: disable ok Button if no entrys in the list
//    if (m_pluginList->count() == 0) close();
//    else done(true);
//}

//int RSelectPluginDialog::getIndex(QWidget *parent, const QStringList &plugins, bool *ok)
//{

//    //qDebug() << plugins;

//    RSelectPluginDialog dialog(plugins, parent);


//    int ret = dialog.exec();
//    if (ok) *ok = !!ret;
//    if (ret) return dialog.m_pluginList->currentRow();
//}
