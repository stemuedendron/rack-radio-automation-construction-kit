#ifndef RBLINKBUTTON_H
#define RBLINKBUTTON_H

#include <QPushButton>
#include <QTimer>

class RBlinkButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool blinking READ blinking)

public:

    explicit RBlinkButton(const QString &text, QWidget *parent = 0);
    int blinking() const {return m_blinking;}

signals:

private slots:

    void startTimer(const bool &);
    void setBlinking();

private:

    bool m_blinking;
    QTimer *m_timer;

};

#endif // RBLINKBUTTON_H
