#ifndef FSPLAYBUTTON_H
#define FSPLAYBUTTON_H
#include "fsclockbutton.h"

class FSPlayButton : public FSClockButton
{
    Q_OBJECT
    
public:

    FSPlayButton(QWidget *parent = 0);

protected:

    void paintEvent(QPaintEvent *ev);
    
};

#endif // FSPLAYBUTTON_H
