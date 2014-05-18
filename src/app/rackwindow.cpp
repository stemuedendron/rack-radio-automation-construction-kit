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

#include "rackwindow.h"
#include "coreimpl.h"
#include "iwidgetplugin.h"
#include "rsplitter.h"
#include "rpushbutton.h"
#include "rblinkbutton.h"
#include "rselectplugindialog.h"
#include "rpreviewwidget.h"

#include <QtWidgets>


//TODO: method: delete one plugin

//TODO: only one qpluginloader per plugin (use member e.g. QList pluginLibs)
// create many pluginwidgets form this loader
//QList plugin widget pointers, connect destroyed signals to remove from list
//and check if we can unload the plugin

//BUG: if we load many plugins each in one pluginhost (splitter) then we have many toolbars
//that make the window width grown


RackWindow::RackWindow() :
    m_coreImpl(new CoreImpl(this)),
    m_mainSplitter(new RSplitter(Qt::Horizontal)),
    m_mapperLoadNewPlugin(new QSignalMapper(this)),
    m_mapperClosePlugin(new QSignalMapper(this)),
    m_mapperClosePluginHost(new QSignalMapper(this))
{

    setContextMenuPolicy(Qt::NoContextMenu);

    m_styleSheetName = ":/stylesheets/default.qss";
    loadStyleSheet();

    m_mainSplitter->setObjectName("rackMainSplitter");
    setCentralWidget(m_mainSplitter);





    //test webkit crash:


//    QPluginLoader pluginLoader("/home/rf/Dokumente/rack-radio-automation-construction-kit/build-release/app/plugins/libwebbrowserplugin.so");
//    QObject *plugin = pluginLoader.instance();

//    if (plugin)
//    {
//        IWidgetPlugin *widgetPlugin = qobject_cast<IWidgetPlugin *>(plugin);
//        if (widgetPlugin)
//        {
//            QWidget *newWidget = widgetPlugin->createRWidget(m_coreImpl, this);
//            setCentralWidget(newWidget);
//        }
//    }






    //end;





    createToolBars();
    createPluginHost(0);

    RPreviewWidget *previewWidget = new RPreviewWidget(this);
    QObject::connect(m_coreImpl, SIGNAL(previewStateChanged(bool)), previewWidget, SLOT(fade(bool)));

    QObject::connect(m_mapperLoadNewPlugin, SIGNAL(mapped(QWidget*)), this, SLOT(loadPlugin(QWidget*)));
    QObject::connect(m_mapperClosePlugin, SIGNAL(mapped(QObject*)), this, SLOT(deletePluginSwitchAction(QObject*)));
    QObject::connect(m_mapperClosePluginHost, SIGNAL(mapped(QWidget*)), this, SLOT(closePluginHost(QWidget*)));
    QObject::connect(this, SIGNAL(enterSettingsMode()), m_mainSplitter, SIGNAL(enterSettingsMode()));
    QObject::connect(this, SIGNAL(leaveSettingsMode()), m_mainSplitter, SIGNAL(leaveSettingsMode()));

    emit leaveSettingsMode();

}

void RackWindow::createToolBars()
{
    //main toolbar actions:
    QAction *fullscreenAct = new QAction(tr("Fullscreen"), this);
    fullscreenAct->setCheckable(true);
    QAction *enterSettingsAct = new QAction(tr("Change Widget Layout"), this);
    QAction *quitAct = new QAction(tr("Quit"), this);

    //main toolbar menus:
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setObjectName("rackMainMenu");
    QAction *titleAct = new QAction(tr("R.A.C.K."),this);
    titleAct->setDisabled(true);
    mainMenu->addAction(titleAct);
    mainMenu->setDefaultAction(titleAct);
    mainMenu->addAction(fullscreenAct);
    mainMenu->addAction(enterSettingsAct);
    mainMenu->addAction(quitAct);

    //main toolbar buttons:
    RPushButton *settingsButton = new RPushButton(tr("Menu"));
    settingsButton->setObjectName("rackSettingsButton");
    settingsButton->setMenu(mainMenu);
    RBlinkButton *deleteButton = new RBlinkButton(tr("Delete"));
    deleteButton->setObjectName("rackDeleteButton");
    RBlinkButton *previewButton = new RBlinkButton(tr("Preview"));
    previewButton->setObjectName("rackPreviewButton");

    //main toolbar:
    QToolBar *mainToolBar = new QToolBar;
    mainToolBar->setObjectName("rackMainToolBar");
    mainToolBar->setMovable(false);
    mainToolBar->addWidget(settingsButton);
    mainToolBar->addWidget(deleteButton);
    mainToolBar->addWidget(previewButton);

    //delete state:
    QObject::connect(deleteButton, SIGNAL(clicked()), m_coreImpl, SLOT(toggleDeleteState()));
    QObject::connect(m_coreImpl, SIGNAL(deleteStateChanged(bool)), deleteButton, SLOT(setBlinking(bool)));

    //preview state:
    QObject::connect(previewButton, SIGNAL(clicked()), m_coreImpl, SLOT(togglePreviewState()));
    QObject::connect(m_coreImpl, SIGNAL(previewStateChanged(bool)), previewButton, SLOT(setBlinking(bool)));

    //normal state:
    QObject::connect(this, SIGNAL(enterSettingsMode()), m_coreImpl, SLOT(setNormalState()));

    //main toolbar signals & slots:
    QObject::connect(fullscreenAct, SIGNAL(triggered(bool)), this, SLOT(toggleFullscreen()));
    QObject::connect(enterSettingsAct, SIGNAL(triggered()), SIGNAL(enterSettingsMode()));
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(this, SIGNAL(enterSettingsMode()), mainToolBar, SLOT(hide()));
    QObject::connect(this, SIGNAL(leaveSettingsMode()), mainToolBar, SLOT(show()));





    /////////////////////////save tests
    QAction *saveAct = new QAction(tr("Save"), this);
    mainMenu->addAction(saveAct);
    QObject::connect(saveAct, SIGNAL(triggered()), this, SLOT(savePluginHosts()));
    ////////////////////////////


    /////////////////////////style sheet load
    QAction *styleAct = new QAction(tr("Load Style Sheet"), this);
    mainMenu->addAction(styleAct);
    QObject::connect(styleAct, SIGNAL(triggered()), this, SLOT(openStyleSheet()));
    ////////////////////////////

    /////////////////////////style sheet reload
    QAction *restyleAct = new QAction(tr("Reload Style Sheet"), this);
    mainMenu->addAction(restyleAct);
    QObject::connect(restyleAct, SIGNAL(triggered()), this, SLOT(loadStyleSheet()));
    ////////////////////////////


    //settings toolbar buttons:
    RPushButton *settingsOKButton = new RPushButton(tr("OK"));
    settingsOKButton->setObjectName("rackSettingsOKButton");

    RPushButton *settingsCancelButton = new RPushButton(tr("Cancel"));
    settingsCancelButton->setObjectName("rackSettingsCancelButton");

    //settings toolbar:
    QToolBar *settingsToolBar = new QToolBar;
    settingsToolBar->setObjectName("rackSettingsToolBar");
    settingsToolBar->setMovable(false);
    settingsToolBar->addWidget(settingsOKButton);
    settingsToolBar->addWidget(settingsCancelButton);

    //settings toolbar signals & slots:
    QObject::connect(settingsOKButton, SIGNAL(clicked()), SIGNAL(leaveSettingsMode()));
    QObject::connect(this, SIGNAL(enterSettingsMode()), settingsToolBar, SLOT(show()));
    QObject::connect(this, SIGNAL(leaveSettingsMode()), settingsToolBar, SLOT(hide()));

    //add toolbars to mainwindow:
    addToolBar(Qt::BottomToolBarArea, mainToolBar);
    addToolBar(Qt::BottomToolBarArea, settingsToolBar);
}

void RackWindow::createPluginHost(int position)
{
    //layout settings widget:
    QWidget *settingsWidget = new QWidget;
    settingsWidget->setPalette(QPalette(QColor(0,0,0,160)));
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

    //vertical toolbar for plugin buttons:
    QToolBar *pluginHostToolBar = new QToolBar;
    pluginHostToolBar->setObjectName("rackPluginHostToolBar");
    pluginHostToolBar->setOrientation(Qt::Vertical);
    //actiongroup for exclusive handling of buttons:
    QActionGroup *ag = new QActionGroup(pluginHostToolBar);
    ag->setExclusive(true);

    RPushButton *addPluginWidgetButton = new RPushButton(tr("Add Widget ..."));
    addPluginWidgetButton->setObjectName("rackAddPluginWidgetButton");

    QWidget *middleWidget = new QWidget;
    QVBoxLayout *middleLayout = new QVBoxLayout(middleWidget);
    middleLayout->setSpacing(0);
    middleLayout->setContentsMargins(0,0,0,0);
    middleLayout->addWidget(pluginHostToolBar, 0, Qt::AlignHCenter);
    middleLayout->addWidget(addPluginWidgetButton, 0, Qt::AlignHCenter);

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
    pluginHost->setMinimumSize(200,80);

    QStackedLayout *overlayLayout = new QStackedLayout(pluginHost);
    overlayLayout->setStackingMode(QStackedLayout::StackAll);
    overlayLayout->addWidget(pluginStack);
    overlayLayout->addWidget(settingsWidget);
    overlayLayout->setCurrentIndex(1);

    //enter/leave settings signals:
    QSignalMapper *mapperShowSettingsMode = new QSignalMapper(pluginHost);
    QObject::connect(this, SIGNAL(enterSettingsMode()), mapperShowSettingsMode, SLOT(map()));
    mapperShowSettingsMode->setMapping(this, 1);
    QSignalMapper *mapperHideSettingsMode = new QSignalMapper(pluginHost);
    QObject::connect(this, SIGNAL(leaveSettingsMode()), mapperHideSettingsMode, SLOT(map()));
    mapperHideSettingsMode->setMapping(this, 0);
    QObject::connect(mapperShowSettingsMode, SIGNAL(mapped(int)), overlayLayout, SLOT(setCurrentIndex(int)));
    QObject::connect(mapperHideSettingsMode, SIGNAL(mapped(int)), overlayLayout, SLOT(setCurrentIndex(int)));

    //create plugin host widget signals:
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

    //load plugin signal:
    QObject::connect(addPluginWidgetButton, SIGNAL(clicked()), m_mapperLoadNewPlugin, SLOT(map()));
    m_mapperLoadNewPlugin->setMapping(addPluginWidgetButton, pluginHost);

    //close plugin host signal:
    QObject::connect(closeButton, SIGNAL(clicked()), m_mapperClosePluginHost, SLOT(map()));
    m_mapperClosePluginHost->setMapping(closeButton, pluginHost);

    //create plugin switch signalmapper
    QSignalMapper *mapperSwitchPlugin = new QSignalMapper(pluginHost);
    mapperSwitchPlugin->setObjectName("rackPluginSwitchMapper");
    QObject::connect(mapperSwitchPlugin, SIGNAL(mapped(QWidget *)), pluginStack, SLOT(setCurrentWidget(QWidget *)));


    ////test show/hide plugin widget

    ////QObject::connect(mapperSwitchPlugin, SIGNAL(mapped(QWidget *)), this, SLOT(showHidePluginWidget(QWidget*)));


    //////


    //create plugin toolbar for mainwindow
    QToolBar *pluginToolBar = new QToolBar;
    pluginToolBar->setObjectName("rackPluginToolBar");
    pluginToolBar->setMovable(false);
    pluginToolBar->hide();
    addToolBar(Qt::BottomToolBarArea, pluginToolBar);

    //store the toolbar pointer as dynamic property to access later when creating plugin toolbar buttons
    pluginHost->setProperty("pluginToolBar", qVariantFromValue((QWidget *)pluginToolBar));

    //plugin bar signals & slots:
    QObject::connect(this, SIGNAL(enterSettingsMode()), pluginToolBar, SLOT(hide()));
    QObject::connect(this, SIGNAL(leaveSettingsMode()), pluginToolBar, SLOT(show()));







    //insert new pluginhost widget in splitter, create new splitter if necessary
    if (position == 0)
    {
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
            QObject::connect(this, SIGNAL(enterSettingsMode()), newSplitter, SIGNAL(enterSettingsMode()));
            QObject::connect(this, SIGNAL(leaveSettingsMode()), newSplitter, SIGNAL(leaveSettingsMode()));
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


//FIXME: keep loaded plugins in own plugin manager and
//unload plugins to let load new version without
//the need to restart the application
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

    if (!pluginsDir.cd("plugins"))
    {
        QMessageBox::information(this, "Error", "No plugin folder found");
        return;
    }


    QStringList pluginList = pluginsDir.entryList(QDir::Files);


    RSelectPluginDialog pluginDialog(this);
    pluginDialog.pluginListWidget->addItems(pluginList);
    pluginDialog.pluginListWidget->setCurrentRow(0);
    if (pluginDialog.exec())
    {



//    bool ok;
//    int newPluginIndex = RSelectPluginDialog::getIndex(this, pluginList, &ok);
//    if (ok) {

        //QString fileName = pluginsDir.entryList(QDir::Files).at(newPluginIndex);

        QString fileName = pluginsDir.entryList(QDir::Files).at(pluginDialog.pluginListWidget->currentRow());

        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();


        ///test

//        QList<QPluginLoader *> loadedPlugins = findChildren<QPluginLoader *>();

//        QObject *plugin = 0;

//        for (int i = 0; i < loadedPlugins.size(); ++i) {
//            if (loadedPlugins.at(i)->fileName() == pluginsDir.absoluteFilePath(fileName)) {
//                plugin = loadedPlugins.at(i)->instance();
//                break;
//            }
//        }

//        if (!plugin) {
//            QPluginLoader *pluginLoader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName), this);
//            plugin = pluginLoader->instance();
//        }


        //debug code
        qDebug() << "we have the following plugins loaded:";
        QList<QPluginLoader *> debugPlugins = findChildren<QPluginLoader *>();
        for (int i = 0; i < debugPlugins.size(); ++i) {
            qDebug() << debugPlugins.at(i)->fileName();
        }


        //////////


//        m_pluginLoader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName), this);
//        QObject *plugin = m_pluginLoader->instance();

        /////////////////


        if (plugin)
        {
            IWidgetPlugin *widgetPlugin = qobject_cast<IWidgetPlugin *>(plugin);
            if (widgetPlugin)
            {
                QWidget *newWidget = widgetPlugin->createRWidget(m_coreImpl, this);

                //get pointers from pluginhost:
                QStackedWidget *pluginStack = pluginHost->findChild<QStackedWidget *>("rackPluginStack");
                QToolBar *pluginHostToolBar = pluginHost->findChild<QToolBar *>("rackPluginHostToolBar");
                QToolBar *pluginToolBar = pluginHost->property("pluginToolBar").value<QToolBar *>();
                QSignalMapper *sm = pluginHost->findChild<QSignalMapper *>("rackPluginSwitchMapper");
                QActionGroup *ag = pluginHostToolBar->findChild<QActionGroup *>();

                //add plugin widget to the widget stack:
                pluginStack->setCurrentIndex(pluginStack->addWidget(newWidget));

                //create action for the toolbars:
                QAction *act = new QAction(widgetPlugin->name(), ag);
                act->setCheckable(true);
                act->setChecked(true);
                //qt bugfix: set transparent dummy icon to move button text down or right
                act->setIcon(QIcon(":/images/transparent-icon.png"));

                //create button for pluginhost toolbar:
                QToolButton *tb = new QToolButton;
                tb->setObjectName(QLatin1String(newWidget->metaObject()->className()) + "ToolButton");
                tb->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
                tb->setFocusPolicy(Qt::NoFocus);
                tb->setDefaultAction(act);
                RPushButton *settingsButton = new RPushButton;
                settingsButton->setObjectName("rackPluginHostToolBarSettingsButton");
                RPushButton *deleteButton = new RPushButton;
                deleteButton->setObjectName("rackPluginHostToolBarDeleteButton");
                QHBoxLayout *hl = new QHBoxLayout(tb);
                hl->setSpacing(0);
                hl->setContentsMargins(0,0,1,0);
                hl->addStretch();
                hl->addWidget(settingsButton);
                hl->addWidget(deleteButton);
                pluginHostToolBar->addWidget(tb);

                //create button for plugin toolbar:
                QToolButton *tb1 = new QToolButton;
                tb1->setObjectName(QLatin1String(newWidget->metaObject()->className()) + "ToolButton");
                tb1->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                tb1->setFocusPolicy(Qt::NoFocus);
                tb1->setDefaultAction(act);
                pluginToolBar->addWidget(tb1);

                //connect action trigger to PluginSwitchMapper;
                QObject::connect(act, SIGNAL(triggered()), sm, SLOT(map()));
                sm->setMapping(act, newWidget);

                //connect delete signal
                //remove act from actiongroup and delete it:
                QObject::connect(deleteButton, SIGNAL(clicked()), m_mapperClosePlugin, SLOT(map()));
                m_mapperClosePlugin->setMapping(deleteButton, act);
                QObject::connect(deleteButton, SIGNAL(clicked()), newWidget, SLOT(deleteLater()));
                QObject::connect(deleteButton, SIGNAL(clicked()), tb1, SLOT(deleteLater()));
                QObject::connect(deleteButton, SIGNAL(clicked()), tb, SLOT(deleteLater()));

                //connect settings signal
                //if (plugin has settings) ...
                //QObject::connect(settingsButton, SIGNAL(clicked()),

            }
        }
        else
        {
            QMessageBox::information(this, "Error", "Could not load the plugin");
            qDebug() << pluginLoader.errorString();

        }
    }
}


void RackWindow::deletePluginSwitchAction(QObject *action)
{
    QAction *act = qobject_cast<QAction *>(action);
    QActionGroup *ag = act->actionGroup();

    //make sure we have a checked action in the actiongroup after we delete this one
    if (act == ag->checkedAction() && ag->actions().count() > 1) {
        if (act == ag->actions().last())
        {
            ag->actions().at(ag->actions().count() - 2)->trigger();
        }
        else
        {
            ag->actions().at(ag->actions().indexOf(act) + 1)->trigger();
        }
    }
    delete act;
    act = 0;
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
        delete pluginHost->property("pluginToolBar").value<QToolBar *>();
        pluginHost->setProperty("pluginToolBar", 0);
        delete pluginHost;
        pluginHost = 0;
        splitter->setSizes(widgetsizes);
    }
    else {
        delete pluginHost->property("pluginToolBar").value<QToolBar *>();
        pluginHost->setProperty("pluginToolBar", 0);
        delete pluginHost;
        pluginHost = 0;
    }
    if (splitter->count()==1 && parentSplitter)
    {
        parentSplitter->insertWidget(parentSplitter->indexOf(splitter), splitter->widget(0));
        delete splitter;
        splitter = 0;
    }


    ///test
//    if (m_pluginLoader->unload()) {

//        delete m_pluginLoader;
//        m_pluginLoader = 0;
//        qDebug() << "unloaded";
//    }

    //////////////////

//    QList<RSplitter *> splitters = findChildren<RSplitter *>();
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

    QList<QObject *> allObjects = findChildren<QObject *>();
    settings.setValue("Alle-Objekte", allObjects.count());
    for (int i = 0; i < allObjects.count(); ++i)
    {
       settings.setValue(allObjects.at(i)->metaObject()->className(), QString::number(i));
      //  if (qobject_cast<RSplitter *>(children().at(i))) allsplitters << qobject_cast<RSplitter *>(children().at(i));
    }




//    QObjectList allObj = children();

//    QList<RSplitter*> allsplitters;

//    for (int i = 0; i < children().count(); ++i)
//    {
//       settings.setValue(children().at(i)->metaObject()->className(), "nix");
//      //  if (qobject_cast<RSplitter *>(children().at(i))) allsplitters << qobject_cast<RSplitter *>(children().at(i));
//    }

//    settings.setValue("AlleSplitter",allsplitters.count());
//    settings.setValue("Alle", allObj.count());





//    QList<QWidget *> allWidgets = findChildren<QWidget *>();


//    for (int i = 0; i < allWidgets.count(); ++i)
//    {
//        settings.setValue(allWidgets.at(i)->metaObject()->className(), i);
//    }

//    settings.beginWriteArray("widgets");

//    for (int i = 0; i < allObj.count(); ++i)
//    {
//      settings.setArrayIndex(i);
//      settings.setValue(allObj.at(i)->metaObject()->className(), "nix");
//    }
//     settings.endArray();

//     settings.beginGroup("splitters/test");

//     // foreach (QObject* child, children()) settings.setValue(child->metaObject()->className(), child->parent()->metaObject()->className());

//     settings.setValue("rwigetcount",allObj.count());
//     settings.endGroup();

//     //nothing to save here but the plugins inside the rMain RWidget
//     //    if (count() == 1)
//     //    {

//     //    }
//     //    //we have at least 2 Rwidgets
//     //    else
//     //    {

//     //    }
//     //get the root widget if we have more than one rwidget (if it is rsplitter):
//     RSplitter *root = qobject_cast<RSplitter *>(layout()->itemAt(0)->widget());
//     if (root)
//     {

//         settings.setValue("splitterSizes", root->saveState());

//         saveSplittertoXML(root);
//         saveSplitter(root);
//     }

}

//Variante 1: XML mit QSettings schreiben und splitter.state sichern:
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

void RackWindow::toggleFullscreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}



void RackWindow::loadStyleSheet()
{
    QFile file(m_styleSheetName);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);
    //qApp->setStyleSheet(styleSheet);
    file.close();
}

void RackWindow::openStyleSheet()
{
    QString fileName(QFileDialog::getOpenFileName(this, tr("Open Style Sheet"), QDir::homePath(), tr("Qt Style Sheet Files (*.qss)")));
    if (!fileName.isEmpty())
    {
        m_styleSheetName = fileName;
        loadStyleSheet();
    }
}




////test show/hide plugin widget

//void RackWindow::showHidePluginWidget(QWidget *pluginWidget)
//{
//    pluginWidget->isVisible() ? pluginWidget->hide() : pluginWidget->show();
//}

////


RackWindow::~RackWindow()
{
    delete m_coreImpl;
    m_coreImpl = 0;
}

