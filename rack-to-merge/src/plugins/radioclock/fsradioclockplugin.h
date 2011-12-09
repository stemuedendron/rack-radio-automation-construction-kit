#ifndef FSRADIOCLOCKPLUGIN_H
#define FSRADIOCLOCKLUGIN_H

#include <QObject>
#include "fsinterface.h"


class FSRadioClockPlugin : public QObject, FSInterface
{
    Q_OBJECT
    Q_INTERFACES(FSInterface)

public:

    QWidget *createFSWidget(QWidget *parent);


};


#endif
