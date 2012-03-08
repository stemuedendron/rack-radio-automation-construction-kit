#ifndef RACK_H
#define RACK_H

#include <QObject>

class Rack : public QObject
{
    Q_OBJECT
    Q_ENUMS(CoreState)

public:

    enum CoreState
    {
        NormalState,
        InsertState,
        PreviewState,
        DeleteState
    };
    
    
};

#endif // RACK_H
