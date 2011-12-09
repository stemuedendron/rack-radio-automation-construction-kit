#include <QtGui>

#include "fsradioclockplugin.h"
#include "fsradioclock.h"

QWidget *FSRadioClockPlugin::createFSWidget(QWidget *parent)
 {

    return new FSRadioClock(parent);

 }


Q_EXPORT_PLUGIN2(fsradioclockplugin, FSRadioClockPlugin);

