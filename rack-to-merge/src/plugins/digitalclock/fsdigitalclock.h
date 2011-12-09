#ifndef FSDIGITALCLOCK_H
#define FSDIGITALCLOCK_H

#include "fsgradbutton.h"

class FSDigitalClock : public FSGradButton
{
    Q_OBJECT

public:

    FSDigitalClock(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:

    bool pushed;

};

#endif // FSDIGITALCLOCK_H
