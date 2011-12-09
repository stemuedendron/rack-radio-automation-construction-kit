#ifndef FSCLOCKBUTTON_H
#define FSCLOCKBUTTON_H
#include "fsgradbutton.h"

class FSClockButton : public FSGradButton
{
    Q_OBJECT

public:

    FSClockButton(QWidget *parent = 0);

public slots:

    void setPositionClock(int);
    void setTextStatus(QString);
    void setFontStatus(QFont);
    void setColorStatus(QColor);
    void setColorText(QColor);

protected:

    void paintEvent(QPaintEvent *);
    int position;

private:

    QString textStatus;
    QFont fontStatus;
    QColor colorStatus;
    QColor colorText;

};

#endif // FSCLOCKBUTTON_H
