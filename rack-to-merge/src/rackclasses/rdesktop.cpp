#include "rdesktop.h"
#include "rpluginhost.h"
#include "rsplitter.h"
#include <QSettings>
#include <QVBoxLayout>

#include <QDir>
#include <QXmlStreamWriter>

#include <QDebug>


/**
 * @brief
 *
 * @param parent
 */
RDesktop::RDesktop(QWidget *parent) : QWidget(parent)
{
    RPluginHost *fsMain = new RPluginHost;
    fsMain->setLayoutVisible(false);
    QObject::connect(this, SIGNAL(changeConfigModus(bool)), fsMain, SLOT(setLayoutVisible(bool)));
    QObject::connect(fsMain, SIGNAL(btNewWidgetCLick(int)), SLOT(btNewFSWidgetClick(int)));
    QObject::connect(fsMain, SIGNAL(btCloseWidgetClick()), SLOT(btCloseFSWidgetClick()));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(fsMain);
    setLayout(layout);
}


/**
 * @brief
 *
 * @param value
 */
void RDesktop::setConfigModus(bool value)
{
    emit changeConfigModus(value);
}


/**
 * @brief
 *
 * @param position
 */
void RDesktop::btNewFSWidgetClick(int position)
{
    RPluginHost *newFSWidget = new RPluginHost;
    QObject::connect(this, SIGNAL(changeConfigModus(bool)), newFSWidget, SLOT(setLayoutVisible(bool)));
    QObject::connect(newFSWidget, SIGNAL(btNewWidgetCLick(int)), SLOT(btNewFSWidgetClick(int)));
    QObject::connect(newFSWidget, SIGNAL(btCloseWidgetClick()), SLOT(btCloseFSWidgetClick()));
    RPluginHost *senderWidget = qobject_cast<RPluginHost *>(sender());
    RSplitter *parentSplitter = qobject_cast<RSplitter *>(sender()->parent());
    if (!parentSplitter)
    {
        RSplitter *newSplitter = new RSplitter(Qt::Horizontal);
        QObject::connect(this, SIGNAL(changeConfigModus(bool)), newSplitter, SLOT(setConfigModus(bool)));
        newSplitter->addWidget(senderWidget);
        layout()->addWidget(newSplitter);
        parentSplitter = newSplitter;
    }
    QList<int> widgetsizes;
    int senderpos = parentSplitter->indexOf(senderWidget);
    int newposition;
    if ((position == -1) or (position == -2)) newposition = senderpos;
    else newposition = senderpos + 1;
    switch (position + parentSplitter->orientation()) {             //horizontal=1 vertical=2
    case 0:                                                         //left   horizontal / top vertical
    case 2:                                                         //right  horizontal
    case 4:                                                         //bottom vertical
        widgetsizes = parentSplitter->sizes();
        widgetsizes.replace(senderpos, int(widgetsizes.at(senderpos)/2));
        widgetsizes.insert(senderpos + 1, widgetsizes.at(senderpos));
        parentSplitter->insertWidget(newposition, newFSWidget);
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
            parentSplitter->insertWidget(newposition, newFSWidget);
            parentSplitter->setSizes(widgetsizes);
        }
        else if (parentSplitter->count() > 1)
        {
            RSplitter *newSplitter = new RSplitter(Qt::Orientation(abs(position)));

            QObject::connect(this, SIGNAL(changeConfigModus(bool)), newSplitter, SLOT(setConfigModus(bool)));

            widgetsizes = parentSplitter->sizes();
            parentSplitter->insertWidget(parentSplitter->indexOf(senderWidget), newSplitter);
            newSplitter->addWidget(senderWidget);
            QList<int> newsizes = newSplitter->sizes();
            newsizes.replace(0, int(newsizes.at(0)/2));
            newsizes.append(newsizes.at(0));
            if ((position == -1) or (position == -2)) newSplitter->insertWidget(0, newFSWidget);
            else newSplitter->addWidget(newFSWidget);
            newSplitter->setSizes(newsizes);
            parentSplitter->setSizes(widgetsizes);
        }
        break;
    }
}


/**
 * @brief
 *
 */
void RDesktop::btCloseFSWidgetClick()
{
    RPluginHost *senderWidget = qobject_cast<RPluginHost *>(sender());
    RSplitter *parentSplitter = qobject_cast<RSplitter *>(sender()->parent());
    if (parentSplitter)
    {
        QList<int> widgetsizes = parentSplitter->sizes();
        int senderpos = parentSplitter->indexOf(senderWidget);
        if (senderpos == widgetsizes.count()-1)
        {
            widgetsizes.replace(senderpos-1, widgetsizes.at(senderpos-1) + widgetsizes.at(senderpos));
        }
        else
        {
            widgetsizes.replace(senderpos, widgetsizes.at(senderpos) + widgetsizes.at(senderpos+1));
            widgetsizes.removeAt(senderpos + 1);
        }
        delete senderWidget;
        parentSplitter->setSizes(widgetsizes);
        if (parentSplitter->count()==1)
        {
            if (parentSplitter->parent()->inherits("RSplitter"))
            {
                RSplitter *parentParent = qobject_cast<RSplitter *>(parentSplitter->parent());
                parentParent->insertWidget(parentParent->indexOf(parentSplitter), parentSplitter->widget(0));
            }
            else
            {
                layout()->addWidget(parentSplitter->widget(0));
            }
            delete parentSplitter;
        }
    }
}


/**
 * @brief
 *
 */
void RDesktop::saveFSWidgets()
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

         settings.setValue("fswigetcount",allObj.count());


         settings.endGroup();

    //nothing to save here but the plugins inside the fsMain FSWidget
//    if (count() == 1)
//    {


//    }
//    //we have at least 2 FSwidgets
//    else
//    {


//    }
         //get the root widget if we have more than one fswidget (if it is fssplitter):
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
void RDesktop::saveSplitter(RSplitter *splitter)
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
void RDesktop::saveSplitterItem(QObject *obj, QXmlStreamWriter *xml)
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
void RDesktop::saveSplittertoXML(RSplitter *splitter)
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
void RDesktop::saveSplitterItemtoXML(QObject *obj, QXmlStreamWriter *xml)
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





