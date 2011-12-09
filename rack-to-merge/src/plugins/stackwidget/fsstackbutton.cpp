#include <QLabel>
#include <QGridLayout>
#include "fsstackbutton.h"

FSStackButton::FSStackButton(QWidget *parent) : FSGradButton(parent)
{
    setMaximumHeight(100);

    lbTitle = new QLabel;
    lbTitle->setFont(QFont("sans", 14, QFont::Bold));

    lbArtist = new QLabel;
    lbArtist->setFont(QFont("sans", 14, QFont::Normal));

    lbComment = new QLabel;
    lbComment->setFont(QFont("sans", 11, QFont::Normal));

    lbTime = new QLabel;
    lbTime->setAlignment(Qt::AlignRight);
    lbTime->setFont(QFont("sans", 14, QFont::Bold));

    lbPlayTime = new QLabel;
    lbPlayTime->setAlignment(Qt::AlignRight);
    lbPlayTime->setFont(QFont("sans", 11, QFont::Normal));

    lbID = new QLabel;
    lbID->setAlignment(Qt::AlignRight);
    lbID->setFont(QFont("sans", 11, QFont::Normal));

    QGridLayout *l = new QGridLayout;
    l->setSpacing(8);
    l->addWidget(lbTitle, 0, 0);
    l->addWidget(lbArtist, 1, 0);
    l->addWidget(lbComment, 2, 0);
    l->addWidget(lbTime, 0, 1);
    l->addWidget(lbPlayTime, 1, 1);
    l->addWidget(lbID, 2, 1);
    setLayout(l);
}

void FSStackButton::setTitle(QString value)
{
    lbTitle->setText(value);
}

void FSStackButton::setArtist(QString value)
{
    lbArtist->setText(value);
}

void FSStackButton::setComment(QString value)
{
    lbComment->setText(value);
}

void FSStackButton::setTime(QString value)
{
    lbTime->setText(value);
}

void FSStackButton::setPlayTime(QString value)
{
    lbPlayTime->setText(value);
}

void FSStackButton::setID(QString value)
{
    lbID->setText(value);
}

QSize FSStackButton::minimumSizeHint() const
{
    return QSize(350, 100);
}

QSize FSStackButton::sizeHint() const
{
    return QSize(450, 100);
}

