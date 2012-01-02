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

#include "rackwindow.h"
#include "coreimpl.h"
#include "iwidgetplugin.h"
#include "rsplitter.h"
#include "rpushbutton.h"

#include <QtGui>

RackWindow::RackWindow(QWidget *parent) :
    QWidget(parent),
    //create the rack api object:
    m_coreImpl(new CoreImpl(this)),
    m_mainSplitter(new RSplitter(Qt::Horizontal)),
    m_mapperLoadNewPlugin(new QSignalMapper(this)),
    m_mapperclosePluginHost(new QSignalMapper(this))
{

    setWindowTitle(tr("R.A.C.K."));

    QFile file(":/stylesheets/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);


    m_mainSplitter->setChildrenCollapsible(false);


    //QObject::connect(this, SIGNAL(changeConfigModus(bool)), m_mainSplitter, SLOT(setConfigModus(bool)));


    createPluginHost(0);
    emit setSettingsMode(0);

    //replace with 'real' menu
    RPushButton *showSettingsButton = new RPushButton("Settings");
    RPushButton *hideSettingsButton = new RPushButton("OK");

    RPushButton *saveButton = new RPushButton("Save");
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(savePluginHosts()));

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(showSettingsButton);
    hl->addWidget(hideSettingsButton);
    hl->addWidget(saveButton);
    ////////////////////////////////////

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setSpacing(0);
    vl->setContentsMargins(0,0,0,0);
    vl->addWidget(m_mainSplitter);
    vl->addLayout(hl);
    setLayout(vl);

    QSignalMapper *mapperSetSettingsMode = new QSignalMapper(this);

    QObject::connect(showSettingsButton, SIGNAL(clicked()),mapperSetSettingsMode,SLOT(map()));
    QObject::connect(hideSettingsButton, SIGNAL(clicked()),mapperSetSettingsMode,SLOT(map()));
    mapperSetSettingsMode->setMapping(showSettingsButton, 1);
    mapperSetSettingsMode->setMapping(hideSettingsButton, 0);
    QObject::connect(mapperSetSettingsMode, SIGNAL(mapped(int)), this, SIGNAL(setSettingsMode(int)));

    QObject::connect(m_mapperLoadNewPlugin, SIGNAL(mapped(QWidget*)), this, SLOT(loadPlugin(QWidget*)));
    QObject::connect(m_mapperclosePluginHost, SIGNAL(mapped(QWidget*)), this, SLOT(closePluginHost(QWidget*)));

}

void RackWindow::createPluginHost(int position)
{
    //create new pluginhost widget
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0,0,0,160));
    QWidget *settingsWidget = new QWidget;
    settingsWidget->setPalette(pal);
    settingsWidget->setAutoFillBackground(true);

    RPushButton *leftButton = new RPushButton;
    leftButton->setObjectName("rackSettingsLeftArrowButton");
    RPushButton *rightButton = new RPushButton;
    rightButton->setObjectName("rackSettingsRightArrowButton");
    RPushButton *topButton = new RPushButton;
    topButton->setObjectName("rackSettingsTopArrowButton");
    RPushButton *bottomButton = new RPushButton;
    bottomButton->setObjectName("rackSettingsBottomArrowButton");
    RPushButton *closeButton = new RPushButton;
    closeButton->setObjectName("rackSettingsCloseButton");
    RPushButton *newWidgetButton = new RPushButton(tr("new Widget ..."));
    newWidgetButton->setFixedHeight(40);

    QWidget *middleWidget = new QWidget;
    QVBoxLayout *middleLayout = new QVBoxLayout(middleWidget);
    middleLayout->setObjectName("rackPluginSettingsLayout");
    middleLayout->setSpacing(0);
    middleLayout->setContentsMargins(0,0,0,0);
    middleLayout->addWidget(newWidgetButton, 0, Qt::AlignHCenter);

    QGridLayout *settingsLayout = new QGridLayout(settingsWidget);
    settingsLayout->setSpacing(0);
    settingsLayout->setContentsMargins(0,0,0,0);
    settingsLayout->addWidget(topButton,0,1, Qt::AlignTop | Qt::AlignHCenter);
    settingsLayout->addWidget(closeButton,0,2, Qt::AlignTop | Qt::AlignRight);
    settingsLayout->addWidget(leftButton,1,0, Qt::AlignLeft);
    settingsLayout->addWidget(rightButton,1,2, Qt::AlignRight);
    settingsLayout->addWidget(bottomButton,2,1, Qt::AlignBottom| Qt::AlignHCenter);
    settingsLayout->addWidget(middleWidget,1,1);

    QStackedWidget *pluginStack = new QStackedWidget;
    pluginStack->setObjectName("rackPluginStack");
    pluginStack->setAutoFillBackground(true);
    QWidget *pluginHost = new QWidget;
    //pluginHost->setMinimumSize(80,80);
    QStackedLayout *overlayLayout = new QStackedLayout(pluginHost);
    overlayLayout->setStackingMode(QStackedLayout::StackAll);
    overlayLayout->addWidget(pluginStack);
    overlayLayout->addWidget(settingsWidget);
    overlayLayout->setCurrentIndex(1);

    //show settings signal:
    QObject::connect(this, SIGNAL(setSettingsMode(int)), overlayLayout, SLOT(setCurrentIndex(int)));

    //create new plugin host widget signals:
    QSignalMapper *mapperCreatePluginHost = new QSignalMapper(pluginHost);
    QObject::connect(leftButton, SIGNAL(clicked()), mapperCreatePluginHost, SLOT(map()));
    QObject::connect(rightButton, SIGNAL(clicked()), mapperCreatePluginHost, SLOT(map()));
    QObject::connect(topButton, SIGNAL(clicked()), mapperCreatePluginHost, SLOT(map()));
    QObject::connect(bottomButton, SIGNAL(clicked()), mapperCreatePluginHost, SLOT(map()));
    mapperCreatePluginHost->setMapping(leftButton, NewSplitterLeft);
    mapperCreatePluginHost->setMapping(rightButton, NewSplitterRight);
    mapperCreatePluginHost->setMapping(topButton, NewSplitterTop);
    mapperCreatePluginHost->setMapping(bottomButton, NewSplitterBottom);
    QObject::connect(mapperCreatePluginHost, SIGNAL(mapped(int)), SLOT(createPluginHost(int)));

    //load new plugin signals:
    QObject::connect(newWidgetButton, SIGNAL(clicked()), m_mapperLoadNewPlugin, SLOT(map()));
    m_mapperLoadNewPlugin->setMapping(newWidgetButton, pluginHost);

    //close plugin host signals:
    QObject::connect(closeButton, SIGNAL(clicked()), m_mapperclosePluginHost, SLOT(map()));
    m_mapperclosePluginHost->setMapping(closeButton, pluginHost);

    //create plugin switch signalmapper
    QSignalMapper *mapperSwitchPlugin = new QSignalMapper(pluginHost);
    mapperSwitchPlugin->setObjectName("rackPluginSwitchMapper");
    QObject::connect(mapperSwitchPlugin, SIGNAL(mapped(int)), pluginStack, SLOT(setCurrentIndex(int)));

    //insert new pluginhost widget in splitter, create new splitter if necessary
    if (position == 0) {
        m_mainSplitter->addWidget(pluginHost);
        return;
    }
    QSignalMapper *sm = qobject_cast<QSignalMapper *>(sender());
    QWidget *senderPluginHost = qobject_cast<QWidget *>(sm->mapping(position)->parent()->parent());
    RSplitter *parentSplitter = qobject_cast<RSplitter *>(senderPluginHost->parent());
    QList<int> widgetsizes;
    int senderpos = parentSplitter->indexOf(senderPluginHost);
    int newposition;
    if ((position == NewSplitterLeft) or (position == NewSplitterTop)) newposition = senderpos;
    else newposition = senderpos + 1;
    switch (position + parentSplitter->orientation()) {             //horizontal=1 vertical=2
    case 0:                                                         //left   horizontal / top vertical
    case 2:                                                         //right  horizontal
    case 4:                                                         //bottom vertical
        widgetsizes = parentSplitter->sizes();
        widgetsizes.replace(senderpos, int(widgetsizes.at(senderpos)/2));
        widgetsizes.insert(senderpos + 1, widgetsizes.at(senderpos));
        parentSplitter->insertWidget(newposition, pluginHost);
        parentSplitter->setSizes(widgetsizes);
        break;
    case  1:                                                        //left  vertical
    case  3:                                                        //right vertical / bottom horizontal
    case -1:                                                        //top   horizontal
        if (parentSplitter->count() == 1)
        {
            parentSplitter->setOrientation(Qt::Orientation(abs(position)));
            widgetsizes = parentSplitter->sizes();
            widgetsizes.replace(0, int(widgetsizes.at(0)/2));
            widgetsizes.append(widgetsizes.at(0));
            parentSplitter->insertWidget(newposition, pluginHost);
            parentSplitter->setSizes(widgetsizes);
        }
        else if (parentSplitter->count() > 1)
        {
            RSplitter *newSplitter = new RSplitter(Qt::Orientation(abs(position)));
            newSplitter->setChildrenCollapsible(false);

            //QObject::connect(this, SIGNAL(changeConfigModus(bool)), newSplitter, SLOT(setConfigModus(bool)));

            widgetsizes = parentSplitter->sizes();
            parentSplitter->insertWidget(parentSplitter->indexOf(senderPluginHost), newSplitter);
            newSplitter->addWidget(senderPluginHost);
            QList<int> newsizes = newSplitter->sizes();
            newsizes.replace(0, int(newsizes.at(0)/2));
            newsizes.append(newsizes.at(0));
            if ((position == NewSplitterLeft) or (position == NewSplitterTop)) newSplitter->insertWidget(0, pluginHost);
            else newSplitter->addWidget(pluginHost);
            newSplitter->setSizes(newsizes);
            parentSplitter->setSizes(widgetsizes);
        }
        break;
    }
}

void RackWindow::loadPlugin(QWidget *pluginHost)
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

    QString fileName = pluginsDir.entryList(QDir::Files).first();
    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = pluginLoader.instance();

    if (plugin) {
        IWidgetPlugin *widgetPlugin = qobject_cast<IWidgetPlugin *>(plugin);
        if (widgetPlugin) {
            QWidget *newWidget = widgetPlugin->createRWidget(this, CoreImpl::instance());
            QStackedWidget *pluginStack = qFindChild<QStackedWidget *>(pluginHost, "rackPluginStack");
            int index = pluginStack->addWidget(newWidget);
            pluginStack->setCurrentIndex(index);

            QVBoxLayout *pluginSettingsLayout = qFindChild<QVBoxLayout *>(pluginHost, "rackPluginSettingsLayout");
            RPushButton *pluginButton = new RPushButton(QString::number(index));
            pluginButton->setFixedHeight(40);
            pluginSettingsLayout->insertWidget(pluginSettingsLayout->count() - 1, pluginButton);

            QSignalMapper *sm = qFindChild<QSignalMapper *>(pluginHost, "rackPluginSwitchMapper");
            QObject::connect(pluginButton, SIGNAL(clicked()), sm, SLOT(map()));
            sm->setMapping(pluginButton, index);

        }
    }
    else QMessageBox::information(this, "Error", "Could not load the plugin");
}

//needs handling if we should delete the plugins or not and save there content
void RackWindow::closePluginHost(QWidget *pluginHost)
{
    RSplitter *splitter = qobject_cast<RSplitter *>(pluginHost->parent());
    RSplitter *parentSplitter = qobject_cast<RSplitter *>(splitter->parent());
    if (!parentSplitter && splitter->count() == 1) return;
    if (splitter->count() > 2)
    {
        QList<int> widgetsizes = splitter->sizes();
        int senderpos = splitter->indexOf(pluginHost);
        if (senderpos == widgetsizes.count()-1)
        {
            widgetsizes.replace(senderpos-1, widgetsizes.at(senderpos-1) + widgetsizes.at(senderpos));
        }
        else
        {
            widgetsizes.replace(senderpos, widgetsizes.at(senderpos) + widgetsizes.at(senderpos+1));
            widgetsizes.removeAt(senderpos + 1);
        }
        delete pluginHost;
        splitter->setSizes(widgetsizes);
    }
    else delete pluginHost;
    if (splitter->count()==1 && parentSplitter)
    {
        parentSplitter->insertWidget(parentSplitter->indexOf(splitter), splitter->widget(0));
        delete splitter;
    }
//    QList<RSplitter *> splitters = qFindChildren<RSplitter *>(this);
//    qDebug("splitter count: %d", splitters.size());
}




//bool RPluginHost::loadPlugin()
// {
//     QDir pluginsDir(qApp->applicationDirPath());
// #if defined(Q_OS_WIN)
//     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//         pluginsDir.cdUp();
// #elif defined(Q_OS_MAC)
//     if (pluginsDir.dirName() == "MacOS") {
//         pluginsDir.cdUp();
//         pluginsDir.cdUp();
//         pluginsDir.cdUp();
//     }
// #endif
//     pluginsDir.cd("plugins");

//     ///////////////nur 1 plugin laden nach xml beschreibung (name oder id)
//     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
//         QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//         QObject *plugin = pluginLoader.instance();

//         if (plugin) {
//             m_widgetPlugin = qobject_cast<IWidgetPlugin *>(plugin);
//             if (m_widgetPlugin)
//                 return true;
//         }
//     }

//     return false;
// }


//void RPluginHost::newPlugin()
//{
//    if (loadPlugin())
//    {
//        emit setPluginsVisible(false);
//        QWidget *newWidget = m_widgetPlugin->createRWidget(this, CoreImpl::instance());
//        newWidget->setWindowTitle("testen");

////        QObject::connect(Rack::instance(), SIGNAL(dateStrChanged(QString)), newplugin, SLOT(setDate(QString)));
////        QObject::connect(Rack::instance(), SIGNAL(timeStrChanged(QString)), newplugin, SLOT(setTime(QString)));
////        QObject::connect(newplugin, SIGNAL(sayHello(QString)), Rack::instance(), SLOT(setWindowTitle(QString)));

//        QObject::connect(this, SIGNAL(setPluginsVisible(bool)), newWidget, SLOT(setVisible(bool)));
//        m_mainLayout->addWidget(newWidget);

//        m_settingsWidget->raise();

//        //signal emit für taskbar und layoutwidget buttons
//        m_pluginWidgetList.append(newWidget);
//        emit pluginAdded();

//        static int dummy;
//        dummy++;

//        RPushButton *newpluginbutton = new RPushButton(QString::number(dummy));

//        newpluginbutton->setFixedHeight(40);
//        m_middleLayout->insertWidget(m_middleLayout->count() - 1, newpluginbutton);

//        QObject::connect(newpluginbutton, SIGNAL(clicked()), newWidget, SLOT(raise()));

//        ////QObject::connect(newpluginbutton,SIGNAL(pressed()),SLOT(removePluginButton(middlelayout->indexOf(newpluginbutton))));

//    }
//    else QMessageBox::information(this, "Error", "Could not load the plugin");
//}


void RackWindow::savePluginHosts()
{
    ////orientation unterscheiden

    QSettings settings("RadioFrei", "Layouts");
    settings.clear();

    QObjectList allObj = children();

    QList<RSplitter*> allsplitters;

    for (int i = 0; i < children().count(); ++i)
    {
       settings.setValue(children().at(i)->metaObject()->className(), "nix");
      //  if (qobject_cast<RSplitter *>(children().at(i))) allsplitters << qobject_cast<RSplitter *>(children().at(i));
    }

    settings.setValue("AlleSplitter",allsplitters.count());
    settings.setValue("Alle", allObj.count());

    QList<QWidget *> allWidgets = qFindChildren<QWidget *>(this);

    for (int i = 0; i < allWidgets.count(); ++i)
    {
        settings.setValue(allWidgets.at(i)->metaObject()->className(), i);
    }

    settings.beginWriteArray("widgets");

    for (int i = 0; i < allObj.count(); ++i)
    {
      settings.setArrayIndex(i);
      settings.setValue(allObj.at(i)->metaObject()->className(), "nix");
    }
     settings.endArray();

     settings.beginGroup("splitters/test");

     // foreach (QObject* child, children()) settings.setValue(child->metaObject()->className(), child->parent()->metaObject()->className());

     settings.setValue("rwigetcount",allObj.count());
     settings.endGroup();

     //nothing to save here but the plugins inside the rMain RWidget
     //    if (count() == 1)
     //    {

     //    }
     //    //we have at least 2 Rwidgets
     //    else
     //    {

     //    }
     //get the root widget if we have more than one rwidget (if it is rsplitter):
     RSplitter *root = qobject_cast<RSplitter *>(layout()->itemAt(0)->widget());
     if (root)
     {

         settings.setValue("splitterSizes", root->saveState());

         saveSplittertoXML(root);
         saveSplitter(root);
     }

}

//Variante 1: XML mit QSettings schreiben und splitter.state sichern:
/**
 * @brief
 *
 * @param splitter
 */
void RackWindow::saveSplitter(RSplitter *splitter)
{
    QString setting;
    QXmlStreamWriter xml(&setting);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xml>");
    xml.writeStartElement("racklayout");
    xml.writeAttribute("version", "1.0");
    saveSplitterItem(splitter, &xml);
    xml.writeEndDocument();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RadioFrei", "Layout");
    settings.clear();
    settings.setValue("layout", setting);
}

/**
 * @brief
 *
 * @param obj
 * @param xml
 */
void RackWindow::saveSplitterItem(QObject *obj, QXmlStreamWriter *xml)
{
    QString className = obj->metaObject()->className();
    xml->writeStartElement("object");
    xml->writeAttribute("class", className);
    if (qobject_cast<RSplitter *>(obj))
    {
        RSplitter *splitter = qobject_cast<RSplitter *>(obj);
        xml->writeAttribute("state", QString::fromLatin1(splitter->saveState().constData(), splitter->saveState().size()));
        for (int i = 0; i < splitter->count(); ++i)
        {
            saveSplitterItem(splitter->widget(i),xml);
        }
    }
    xml->writeEndElement();
}


//Variante 2: XML in eigene Datei schreiben und orientation und sizes extra sichern:

/**
 * @brief
 *
 * @param splitter
 */
void RackWindow::saveSplittertoXML(RSplitter *splitter)
{
    QXmlStreamWriter xml;
    xml.setAutoFormatting(true);
    //needs change!!!!!! for cross platform
    QFile file("/home/rf/rack.xml");
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error xml";
        return;
    }
    xml.setDevice(&file);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xml>");
    xml.writeStartElement("racklayout");
    xml.writeAttribute("version", "1.0");
    saveSplitterItemtoXML(splitter, &xml);
    xml.writeEndDocument();
}


/**
 * @brief
 *
 * @param obj
 * @param xml
 */
void RackWindow::saveSplitterItemtoXML(QObject *obj, QXmlStreamWriter *xml)
{
    QString className = obj->metaObject()->className();
    xml->writeStartElement("object");
    xml->writeAttribute("class", className);
    if (qobject_cast<RSplitter *>(obj))
    {
        RSplitter *splitter = qobject_cast<RSplitter *>(obj);
        xml->writeAttribute("orientation", QString::number(splitter->orientation()));
        QList<int> sizes = splitter->sizes();
        QStringList tmp;
        foreach (int size, sizes) tmp << QString::number(size);
        xml->writeAttribute("sizes", tmp.join("|"));
        for (int i = 0; i < splitter->count(); ++i)
        {
            saveSplitterItemtoXML(splitter->widget(i),xml);
        }
    }
    xml->writeEndElement();
}



RackWindow::~RackWindow()
{
    delete m_coreImpl;
    m_coreImpl = 0;
}

