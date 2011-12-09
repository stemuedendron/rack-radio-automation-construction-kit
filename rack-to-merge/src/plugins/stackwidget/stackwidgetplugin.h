#ifndef STACKWIDGETPLUGIN_H
#define STACKWIDGETPLUGIN_H

#include <QObject>
#include "fsinterface.h"


class StackWidgetPlugin : public QObject, FSInterface
{
    Q_OBJECT
    Q_INTERFACES(FSInterface)

public:

    QWidget *createFSWidget(QWidget *parent);


};


#endif
