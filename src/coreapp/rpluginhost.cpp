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

#include "rackimpl.h"
#include "rpluginhost.h"

#include <QtGui>
#include <QTime>

RPluginHost::RPluginHost(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(QSize(120,120));

    //widgets for settings

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0,0,0,160));
    m_wgSettings = new QWidget(this);
    m_wgSettings->setPalette(pal);
    m_wgSettings->setAutoFillBackground(true);

    m_btLeft = new QPushButton;
    m_btLeft->setObjectName("rackSettingsLeftArrowButton");

    m_btRight = new QPushButton;
    m_btRight->setObjectName("rackSettingsRightArrowButton");

    m_btTop = new QPushButton;
    m_btTop->setObjectName("rackSettingsTopArrowButton");

    m_btBottom = new QPushButton;
    m_btBottom->setObjectName("rackSettingsBottomArrowButton");

    m_btClose = new QPushButton;
    m_btClose->setObjectName("rackSettingsCloseButton");


    m_btMiddle = new QPushButton;
    m_btMiddle->setFixedHeight(40);
    m_btMiddle->resize(200,40);

    m_btMiddle->setText(QTime::currentTime().toString(tr("ss")));

    m_vlMiddle = new QVBoxLayout;
    m_vlMiddle->setSpacing(0);
    m_vlMiddle->setContentsMargins(0,0,0,0);
    m_vlMiddle->addWidget(m_btMiddle);

    QWidget *middleWidget = new QWidget;
    middleWidget->setLayout(m_vlMiddle);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_btTop,0,1, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(m_btClose,0,2, Qt::AlignTop | Qt::AlignRight);
    layout->addWidget(m_btLeft,1,0, Qt::AlignLeft);
    layout->addWidget(m_btRight,1,2, Qt::AlignRight);
    layout->addWidget(m_btBottom,2,1, Qt::AlignBottom| Qt::AlignHCenter);
    layout->addWidget(middleWidget,1,1);
    m_wgSettings->setLayout(layout);

    m_signalMapper = new QSignalMapper(this);
    QObject::connect(m_btLeft, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btRight, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btTop, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_btBottom, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(m_btLeft,  -1);
    m_signalMapper->setMapping(m_btRight,  1);
    m_signalMapper->setMapping(m_btTop,   -2);
    m_signalMapper->setMapping(m_btBottom, 2);
    QObject::connect(m_signalMapper, SIGNAL(mapped(int)), SIGNAL(btNewWidgetCLick(int)));
    QObject::connect(m_btClose, SIGNAL(clicked()), SIGNAL(btCloseWidgetClick()));
    QObject::connect(m_btMiddle, SIGNAL(clicked()), SLOT(newPlugin()));

    m_hlMain = new QHBoxLayout;
    m_hlMain->setSpacing(0);
    m_hlMain->setContentsMargins(0,0,0,0);
    setLayout(m_hlMain);

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
             m_rwidgetInterface = qobject_cast<RWidgetInterface *>(plugin);
             if (m_rwidgetInterface)
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

        QWidget *newplugin = m_rwidgetInterface->createRWidget(this, RackImpl::instance());




//        QObject::connect(Rack::instance(), SIGNAL(dateStrChanged(QString)), newplugin, SLOT(setDate(QString)));
//        QObject::connect(Rack::instance(), SIGNAL(timeStrChanged(QString)), newplugin, SLOT(setTime(QString)));
//        QObject::connect(newplugin, SIGNAL(sayHello(QString)), Rack::instance(), SLOT(setWindowTitle(QString)));





        QObject::connect(this, SIGNAL(setPluginsVisible(bool)), newplugin, SLOT(setVisible(bool)));
        m_hlMain->addWidget(newplugin);


        m_wgSettings->raise();

        //signal emit für taskbar und layoutwidget buttons
        m_wlPluginWidgets.append(newplugin);
        emit pluginAdded();



        QPushButton *newpluginbutton = new QPushButton;

        newpluginbutton->setFixedHeight(40);
        m_vlMiddle->insertWidget(0,newpluginbutton);

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
    m_wgSettings->setVisible(value);
}

void RPluginHost::resizeEvent(QResizeEvent *ev)
{
    m_wgSettings->resize(ev->size());
    ev->accept();
}
