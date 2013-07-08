#ifndef RMESSAGE_H
#define RMESSAGE_H

#include <QObject>
#include "rackdclientsocket.h"

class RMessage
{

public:

    RMessage();
    //class_name ( const class_name & )

    RackdClientSocket*client;
    QString command;
    quint8 device;
    QString uri;
    quint32 time;
    quint32 handle;
    bool ok;
    
signals:
    
public slots:
    
};

#endif // RMESSAGE_H
