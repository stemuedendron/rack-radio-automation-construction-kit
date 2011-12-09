#include <QPainter>
#include <QtGui/qevent.h>
#include "fsgradbutton.h"

FSGradButton::FSGradButton(QWidget *parent) : QWidget(parent)
{
    pushed = false;
    focus = false;
    colBut = "#367A0D";
}

void FSGradButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QLinearGradient linGrad(0,0,0,1);
    linGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
    QColor light, dark, colpen;
    if (focus == false)
    {
        if (colBut == Qt::black)
        {
            light = colpen = "#606060";
        }
        else
        {
            light = colpen = colBut.lighter(170);
        }
        dark = colBut;
    }
    if (focus == true)
    {
        if (colBut == Qt::black)
        {
            light = colpen = "#909090";
            dark = "#101010";
        }
        else
        {
            light = colpen = colBut.lighter(220);
            dark = colBut.lighter(150);
        }
        if (pushed == true)
        {
            colpen = Qt::white;
        }
    }
    linGrad.setColorAt(0, light);
    linGrad.setColorAt(1, dark);
    painter.setBrush(linGrad);
    painter.setPen(QPen(colpen, 2));
    painter.drawRoundedRect(2, 2, width()-4, height()-4, 6, 6);

    painter.drawPixmap((width() - pixmap.width()) / 2, (height() - pixmap.height()) / 2, pixmap);

}

void FSGradButton::setColorButton(QColor colorBt)
{
    colBut = colorBt;
    update();
}

QColor FSGradButton::getColor()
{
    return colBut;
}

void FSGradButton::setPixmap(QPixmap apixmap)
{
    pixmap = apixmap;
}

QSize FSGradButton::minimumSizeHint() const
{
    return QSize(40, 40);
}

QSize FSGradButton::sizeHint() const
{
    return QSize(100, 100);
}

void FSGradButton::mousePressEvent(QMouseEvent *ev)
{
    if (QRect(2,2,width()-4, height()-4).contains(ev->x(),ev->y(),true))
    {
        pushed = true;
        update();
    }
}

void FSGradButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if (QRect(2,2,width()-4, height()-4).contains(ev->x(),ev->y(),true))
    {
        pushed = false;
        update();
        emit pressed();
    }
}

void FSGradButton::enterEvent(QEvent *)
{
    focus = true;
    update();
}

void FSGradButton::leaveEvent(QEvent *)
{
    focus = false;
    pushed = false;
    update();
}
