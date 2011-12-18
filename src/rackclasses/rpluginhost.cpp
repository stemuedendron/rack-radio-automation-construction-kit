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

#include "rpluginhost.h"

#include <QtGui>
#include <QTime>

RPluginHost::RPluginHost(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(QSize(120,120));



    //widgets for settings

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0,0,0,160));
    m_layoutWidget = new QWidget(this);
    m_layoutWidget->setPalette(pal);
    m_layoutWidget->setAutoFillBackground(true);

    //btLeft = new QPushButton(QPixmap(":/trolltech/styles/commonstyle/images/up-32.png"), "X");
    m_btLeft = new QPushButton;

    m_btLeft->setFixedSize(40,40);

    m_btRight = new QPushButton;

    m_btRight->setFixedSize(40,40);

    m_btTop = new QPushButton;

    m_btTop->setFixedSize(40,40);

    m_btBottom = new QPushButton;

    m_btBottom->setFixedSize(40,40);

    m_btClose = new QPushButton;

    m_btClose->setFixedSize(40,40);

    m_btMiddle = new QPushButton;
    m_btMiddle->setFixedHeight(40);
    m_btMiddle->resize(200,40);


    m_btMiddle->setText(QTime::currentTime().toString(tr("ss")));


    m_middlelayout = new QVBoxLayout;
    m_middlelayout->setSpacing(0);
    m_middlelayout->setContentsMargins(0,0,0,0);
    m_middlelayout->addWidget(m_btMiddle);

    QWidget *middleWidget = new QWidget;
    middleWidget->setLayout(m_middlelayout);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_btTop,0,1, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(m_btClose,0,2, Qt::AlignTop | Qt::AlignRight);
    layout->addWidget(m_btLeft,1,0, Qt::AlignLeft);
    layout->addWidget(m_btRight,1,2, Qt::AlignRight);
    layout->addWidget(m_btBottom,2,1, Qt::AlignBottom| Qt::AlignHCenter);
    layout->addWidget(middleWidget,1,1);
    m_layoutWidget->setLayout(layout);

    m_signalMapper = new QSignalMapper(this);
    QObject::connect(m_btLeft, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btRight, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btTop, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btBottom, SIGNAL(pressed()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(m_btLeft,  -1);
    m_signalMapper->setMapping(m_btRight,  1);
    m_signalMapper->setMapping(m_btTop,   -2);
    m_signalMapper->setMapping(m_btBottom, 2);
    QObject::connect(m_signalMapper, SIGNAL(mapped(int)), SIGNAL(btNewWidgetCLick(int)));
    QObject::connect(m_btClose, SIGNAL(pressed()), SIGNAL(btCloseWidgetClick()));
    QObject::connect(m_btMiddle, SIGNAL(pressed()), SLOT(newPlugin()));

    m_mainlayout = new QHBoxLayout;
    m_mainlayout->setSpacing(0);
    m_mainlayout->setContentsMargins(0,0,0,0);
    setLayout(m_mainlayout);

}

bool RPluginHost::loadPlugin()
 {
     QDir pluginsDir(qApp->applicationDirPath());
 #if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
         pluginsDir.cdUp();
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
 #endif
     pluginsDir.cd("plugins");

     ///////////////nur 1 plugin laden nach xml beschreibung (name oder id)
     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
         QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
         QObject *plugin = pluginLoader.instance();

         if (plugin) {
             m_rinterface = qobject_cast<RInterface *>(plugin);
             if (m_rinterface)
                 return true;
         }
     }

     return false;
 }


//stackedwidget nutzen!!!!!!!!! (ist dahinter liegendes sichtbar bei transparenz?? wenn ja
//layoutwidget auch mit stackedwidget realisieren
void RPluginHost::newPlugin()
{

    if (loadPlugin())
    {

        emit setPluginsVisible(false);

        QWidget *newplugin = m_rinterface->createRWidget(this);


        QObject::connect(this, SIGNAL(setPluginsVisible(bool)), newplugin, SLOT(setVisible(bool)));
        m_mainlayout->addWidget(newplugin);


        m_layoutWidget->raise();

        //signal emit für taskbar und layoutwidget buttons
        m_wlPluginWidgets.append(newplugin);
        emit pluginAdded();



        QPushButton *newpluginbutton = new QPushButton;

        newpluginbutton->setFixedHeight(40);
        m_middlelayout->insertWidget(0,newpluginbutton);

        ////QObject::connect(newpluginbutton,SIGNAL(pressed()),SLOT(removePluginButton(middlelayout->indexOf(newpluginbutton))));


    }
    else QMessageBox::information(this, "Error", "Could not load the plugin");
}

void RPluginHost::showPlugin(int value)
{
    emit setPluginsVisible(false);
    m_wlPluginWidgets.at(value)->setVisible(true);
}

void RPluginHost::removePlugin(int value)
{

    //wlPluginWidgets.takeAt(value)->deleteLater();
    delete m_wlPluginWidgets.takeAt(value);
}


void RPluginHost::setLayoutVisible(bool value)
{
    m_layoutWidget->setVisible(value);
}

void RPluginHost::resizeEvent(QResizeEvent *ev)
{
    m_layoutWidget->resize(ev->size());
    ev->accept();
}
