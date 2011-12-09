#ifndef FSRADIOCLOCK_H
#define FSRADIOCLOCK_H

#include "fsgradbutton.h"

class FSRadioClock : public FSGradButton
{
    Q_OBJECT

public:

    FSRadioClock(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:

    bool pushed;

};

#endif // FSRADIOCLOCK_H
