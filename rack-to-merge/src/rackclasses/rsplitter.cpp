#include <QtGui>
#include "rsplitter.h"


RSplitterHandle::RSplitterHandle(Qt::Orientation orientation, RSplitter *parent) : QSplitterHandle(orientation, parent)
{
    m_handleColor=Qt::white;
}

void RSplitterHandle::paintEvent(QPaintEvent *event)
{
    if (isEnabled())
    {
    QPainter painter(this);
    painter.fillRect(event->rect(), m_handleColor);
    }
}

void RSplitterHandle::mousePressEvent(QMouseEvent *)
{
    m_handleColor=Qt::red;
    update();
}

void RSplitterHandle::mouseReleaseEvent(QMouseEvent *)
{
    m_handleColor=Qt::white;
    update();
}




RSplitter::RSplitter(Qt::Orientation orientation, QWidget *parent) : QSplitter(orientation, parent)
{
    setChildrenCollapsible(false);
    setHandleWidth(3);
}

QSplitterHandle *RSplitter::createHandle()
{
    RSplitterHandle *myhandle = new RSplitterHandle(orientation(), this);
    QObject::connect(this, SIGNAL(changeConfigModus(bool)), myhandle, SLOT(setEnabled(bool)));
    return myhandle;
}

void RSplitter::setConfigModus(bool value)
{
    emit changeConfigModus(value);
}
