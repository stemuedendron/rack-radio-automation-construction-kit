#include <QPainter>
#include "cmath"
#include "fsclockbutton.h"

FSClockButton::FSClockButton(QWidget *parent) : FSGradButton(parent)
{
    position = 0;
}

void FSClockButton::paintEvent(QPaintEvent *)
{
    if (position != 0)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255,255,255,110));
        int radius = round(sqrt(pow(width(), 2) + pow(height(), 2))) / 2;
        QPainterPath path;
        path.addRoundedRect(2, 2, width()-4, height()-4, 6, 6);
        painter.setClipPath(path);
        painter.drawPie(width() / 2 - radius, height() / 2 - radius, 2 * radius, 2 * radius, 90*16, -position * 16);
        painter.setFont(fontStatus);
        painter.setBrush(colorStatus);
        painter.setPen(QPen(QColor(0,0,0,70), 4));
        QRect rectStatus = painter.boundingRect(2, 2, width()-4, height()-4, Qt::AlignCenter, textStatus);
        rectStatus.adjust(-2, 0, 4, 0);
        painter.drawRoundedRect(rectStatus, 4, 4);
        painter.setPen(QPen(colorText));
        painter.drawText(2, 2, width()-4, height()-4, Qt::AlignCenter, textStatus);
    }
}

void FSClockButton::setPositionClock(int apos)
{
    position = apos;
    update();
}

void FSClockButton::setTextStatus(QString atext)
{
    textStatus = atext;
    update();
}

void FSClockButton::setFontStatus(QFont afont)
{
    fontStatus = afont;
    update();
}

void FSClockButton::setColorStatus(QColor statcolor)
{
    colorStatus = statcolor;
    update();
}

void FSClockButton::setColorText(QColor txtcolor)
{
    colorText = txtcolor;
    update();
}
