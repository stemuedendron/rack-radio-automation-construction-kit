#include "rblinkbutton.h"
#include <QTimer>
#include <QVariant>
#include <QStyle>


RBlinkButton::RBlinkButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    setCheckable(true);
    m_blinking = false;
    m_timer = new QTimer(this);
    m_timer->setInterval(300);
    QObject::connect(this, SIGNAL(toggled(bool)), this, SLOT(startTimer(bool)));
    QObject::connect(m_timer,SIGNAL(timeout()),this,SLOT(setBlinking()));
}


void RBlinkButton::startTimer(const bool &start)
{
    if (start)
    {
        m_timer->start();
    }
    else
    {
        m_timer->stop();
        m_blinking = false;
        style()->unpolish(this);
        style()->polish(this);
    }

}

void RBlinkButton::setBlinking()
{
    m_blinking ? m_blinking = false : m_blinking = true;
    style()->unpolish(this);
    style()->polish(this);
}
