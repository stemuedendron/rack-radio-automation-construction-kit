#include <QtGui>

#include "fsdigitalclockplugin.h"
#include "fsdigitalclock.h"

QWidget *FSDigitalClockPlugin::createFSWidget(QWidget *parent)
 {
    return new FSDigitalClock(parent);
 }


Q_EXPORT_PLUGIN2(fsdigitalclockplugin, FSDigitalClockPlugin)

