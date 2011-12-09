#include <QtGui>

#include "stackwidgetplugin.h"
#include "fsstackwidget.h"

QWidget *StackWidgetPlugin::createFSWidget(QWidget *parent)
 {
//    QWidget *w = new QWidget(parent);

//    QLabel *l = new QLabel(w);
//    l->setText("super dupper!");
//    //return new QWidget(parent);
//    return w;

    return new FSStackWidget(parent);

 }


Q_EXPORT_PLUGIN2(stackwidgetplugin, StackWidgetPlugin);

