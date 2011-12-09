#include <QResizeEvent>
#include <QHBoxLayout>
#include <QDir>
#include <QPluginLoader>
#include <QSignalMapper>
#include <QMessageBox>
#include <QApplication>

#include "rpluginhost.h"
#include "fsgradbutton.h"
#include "fscartbutton.h"






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
    m_btLeft = new FSGradButton;
    m_btLeft->setColorButton(Qt::black);
    m_btLeft->setPixmap(QPixmap(":/images/left.png"));
    m_btLeft->setFixedSize(40,40);

    m_btRight = new FSGradButton;
    m_btRight->setColorButton(Qt::black);
    m_btRight->setPixmap(QPixmap(":/images/right.png"));
    m_btRight->setFixedSize(40,40);

    m_btTop = new FSGradButton;
    m_btTop->setColorButton(Qt::black);
    m_btTop->setPixmap(QPixmap(":/images/top.png"));
    m_btTop->setFixedSize(40,40);

    m_btBottom = new FSGradButton;
    m_btBottom->setColorButton(Qt::black);
    m_btBottom->setPixmap(QPixmap(":/images/bottom.png"));
    m_btBottom->setFixedSize(40,40);

    m_btClose = new FSGradButton;
    m_btClose->setPixmap(QPixmap(":/images/close.png"));
    m_btClose->setColorButton(Qt::black);
    m_btClose->setFixedSize(40,40);

    m_btMiddle = new FSCartButton;
    m_btMiddle->setFixedHeight(40);
    m_btMiddle->resize(200,40);
    m_btMiddle->setColorButton("#244BB0");





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
             m_fsInterface = qobject_cast<FSInterface *>(plugin);
             if (m_fsInterface)
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

        QWidget *newplugin = m_fsInterface->createFSWidget(this);


        QObject::connect(this, SIGNAL(setPluginsVisible(bool)), newplugin, SLOT(setVisible(bool)));
        m_mainlayout->addWidget(newplugin);


        m_layoutWidget->raise();

        //signal emit für taskbar und layoutwidget buttons
        m_wlPluginWidgets.append(newplugin);
        emit pluginAdded();



        FSCartButton *newpluginbutton = new FSCartButton;
        newpluginbutton->setColorButton(Qt::darkGray);
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
