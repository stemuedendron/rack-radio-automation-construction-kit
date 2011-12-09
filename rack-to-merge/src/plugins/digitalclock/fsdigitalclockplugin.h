#ifndef FSDIGITALCLOCKPLUGIN_H
#define FSDIGITALCLOCKLUGIN_H

#include <QObject>
#include "fsinterface.h"


class FSDigitalClockPlugin : public QObject, FSInterface
{
    Q_OBJECT
    Q_INTERFACES(FSInterface)

public:

    QWidget *createFSWidget(QWidget *parent);


};


#endif
