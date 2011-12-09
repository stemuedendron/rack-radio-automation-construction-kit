#include <QPainter>
#include "fscartbutton.h"


FSCartButton::FSCartButton(QWidget *parent) : FSClockButton(parent)
{
    setColorStatus(Qt::yellow);
    setTextStatus(tr("00:32"));
    setFontStatus(QFont("Sans Serif", 15, QFont::Bold));
    setColorText(Qt::black);
}

void FSCartButton::paintEvent(QPaintEvent *ev)
{
    //paint gradient
    FSGradButton::paintEvent(ev);

    //paint labels
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(QFont("Arial", 15, QFont::Bold));
    painter.setPen(QPen(Qt::black));

    painter.drawText(2, 2, width()-4, height()-4, Qt::AlignCenter, Atext);

    //paint position clock
    FSClockButton::paintEvent(ev);

}


void FSCartButton::setText(QString atext)
{
    Atext = atext;
    update();
}
