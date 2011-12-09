#ifndef FSSTACKBUTTON_H
#define FSSTACKBUTTON_H
#include "fsgradbutton.h"

class QLabel;

class FSStackButton : public FSGradButton
{
    Q_OBJECT

public:

    FSStackButton(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:

    void setTitle(QString);
    void setArtist(QString);
    void setComment(QString);
    void setTime(QString);
    void setPlayTime(QString);
    void setID(QString);

private:

    QLabel *lbTitle;
    QLabel *lbArtist;
    QLabel *lbComment;
    QLabel *lbTime;
    QLabel *lbPlayTime;
    QLabel *lbID;

};

#endif // FSSTACKBUTTON_H
