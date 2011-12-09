#ifndef FSCARTBUTTON_H
#define FSCARTBUTTON_H
#include "fsclockbutton.h"

class FSCartButton : public FSClockButton
{
    Q_OBJECT

public:

    FSCartButton(QWidget *parent = 0);

public slots:

    void setText(QString);

protected:

    void paintEvent(QPaintEvent *);

private:

    QString Atext;

};

#endif // FSCARTBUTTON_H
