#include <QPainter>
#include <QBitmap>
#include "fsplaybutton.h"

FSPlayButton::FSPlayButton(QWidget *parent) : FSClockButton(parent)
{
    setColorStatus(Qt::red);
    setTextStatus(tr("ON AIR"));
    setFontStatus(QFont("Arial", 15, QFont::Bold));
    setColorText(Qt::white);
    setFixedSize(100,100);
}

void FSPlayButton::paintEvent(QPaintEvent *ev)
{
    // move this to the state logic (play, delete, move insert ...)
    if (position != 0) setPixmap(0);

    //paint gradient
    FSGradButton::paintEvent(ev);
    //paint position clock
    FSClockButton::paintEvent(ev);
}


