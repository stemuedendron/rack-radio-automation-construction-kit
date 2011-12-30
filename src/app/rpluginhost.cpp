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

#include "coreimpl.h"
#include "rpluginhost.h"

#include <QtGui>
#include <QTime>

RPluginHost::RPluginHost(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(QSize(120,120));

    //widgets for settings

    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0,0,0,160));
    m_settingsWidget = new QWidget(this);
    m_settingsWidget->setPalette(pal);
    m_settingsWidget->setAutoFillBackground(true);

    m_leftButton = new QPushButton;
    m_leftButton->setObjectName("rackSettingsLeftArrowButton");

    m_rightButton = new QPushButton;
    m_rightButton->setObjectName("rackSettingsRightArrowButton");

    m_topButton = new QPushButton;
    m_topButton->setObjectName("rackSettingsTopArrowButton");

    m_bottomButtom = new QPushButton;
    m_bottomButtom->setObjectName("rackSettingsBottomArrowButton");

    m_closeButton = new QPushButton;
    m_closeButton->setObjectName("rackSettingsCloseButton");


    m_newWidgetButton = new QPushButton;
    m_newWidgetButton->setFixedHeight(40);
    m_newWidgetButton->resize(100,40);

    m_newWidgetButton->setText(tr("new Widget ..."));

    m_middleLayout = new QVBoxLayout;
    m_middleLayout->setSpacing(0);
    m_middleLayout->setContentsMargins(0,0,0,0);
    m_middleLayout->addWidget(m_newWidgetButton, 0, Qt::AlignHCenter);

    QWidget *middleWidget = new QWidget;
    middleWidget->setLayout(m_middleLayout);

    QGridLayout *layout = new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_topButton,0,1, Qt::AlignTop | Qt::AlignHCenter);
    layout->addWidget(m_closeButton,0,2, Qt::AlignTop | Qt::AlignRight);
    layout->addWidget(m_leftButton,1,0, Qt::AlignLeft);
    layout->addWidget(m_rightButton,1,2, Qt::AlignRight);
    layout->addWidget(m_bottomButtom,2,1, Qt::AlignBottom| Qt::AlignHCenter);
    layout->addWidget(middleWidget,1,1);
    m_settingsWidget->setLayout(layout);

    m_signalMapper = new QSignalMapper(this);
    QObject::connect(m_leftButton, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_rightButton, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_topButton, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    QObject::connect(m_bottomButtom, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(m_leftButton,  -1);
    m_signalMapper->setMapping(m_rightButton,  1);
    m_signalMapper->setMapping(m_topButton,   -2);
    m_signalMapper->setMapping(m_bottomButtom, 2);
    QObject::connect(m_signalMapper, SIGNAL(mapped(int)), SIGNAL(btNewWidgetCLick(int)));
    QObject::connect(m_closeButton, SIGNAL(clicked()), SIGNAL(btCloseWidgetClick()));
    QObject::connect(m_newWidgetButton, SIGNAL(clicked()), SLOT(newPlugin()));

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0,0,0,0);
    setLayout(m_mainLayout);

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
             m_widgetPlugin = qobject_cast<IWidgetPlugin *>(plugin);
             if (m_widgetPlugin)
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

        QWidget *newWidget = m_widgetPlugin->createRWidget(this, CoreImpl::instance());


        newWidget->setWindowTitle("testen");



//        QObject::connect(Rack::instance(), SIGNAL(dateStrChanged(QString)), newplugin, SLOT(setDate(QString)));
//        QObject::connect(Rack::instance(), SIGNAL(timeStrChanged(QString)), newplugin, SLOT(setTime(QString)));
//        QObject::connect(newplugin, SIGNAL(sayHello(QString)), Rack::instance(), SLOT(setWindowTitle(QString)));





        QObject::connect(this, SIGNAL(setPluginsVisible(bool)), newWidget, SLOT(setVisible(bool)));
        m_mainLayout->addWidget(newWidget);


        m_settingsWidget->raise();

        //signal emit für taskbar und layoutwidget buttons
        m_pluginWidgetList.append(newWidget);
        emit pluginAdded();


        static int dummy;
        dummy++;



        QPushButton *pluginButton = new QPushButton(QString::number(dummy));

        pluginButton->setFixedHeight(40);
        m_middleLayout->insertWidget(m_middleLayout->count() - 1, pluginButton);


        ////QObject::connect(newpluginbutton,SIGNAL(pressed()),SLOT(removePluginButton(middlelayout->indexOf(newpluginbutton))));


    }
    else QMessageBox::information(this, "Error", "Could not load the plugin");
}

void RPluginHost::showPlugin(int value)
{
    emit setPluginsVisible(false);
    m_pluginWidgetList.at(value)->setVisible(true);
}

void RPluginHost::removePlugin(int value)
{

    //wlPluginWidgets.takeAt(value)->deleteLater();
    delete m_pluginWidgetList.takeAt(value);
}


void RPluginHost::setLayoutVisible(bool value)
{
    m_settingsWidget->setVisible(value);
}

void RPluginHost::resizeEvent(QResizeEvent *ev)
{
    m_settingsWidget->resize(ev->size());
    ev->accept();
}
