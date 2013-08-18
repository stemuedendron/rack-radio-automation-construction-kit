/*
    Copyright (C) 2012, Steffen Müller.
    All rights reserved.
    
    This file is part of r.a.c.k. radio automation construction kit
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    Author: Steffen Müller
*/

#include "rpreviewwidget.h"
#include "rpushbutton.h"

#include <QtWidgets>

RPreviewWidget::RPreviewWidget(QWidget *parent) :
    QWidget(parent),
    m_ani(new QPropertyAnimation(this, "pos", this)),
    m_in(false)
{
    setObjectName("rackPreviewWidget");
    qApp->installEventFilter(this);
    m_ani->setDuration(50);


    QLabel *label = new QLabel("Song, Band");
    label->setObjectName("rackPreviewWidgetLabel");

    QWidget *boxLeft = new QWidget;
    boxLeft->setObjectName("rackPreviewWidgetBox");
    QHBoxLayout *boxLeftLayout = new QHBoxLayout(boxLeft);
    QWidget *timeBox = new QWidget;
    timeBox->setObjectName("rackPreviewWidgetTimeBox");
    QGridLayout *timeBoxLayout = new QGridLayout(timeBox);
    QLabel *timeLabel = new QLabel("00:03:52");
    timeLabel->setObjectName("rackPreviewWidgetTimeLabel");
    QLabel *msLabel = new QLabel("300");
    msLabel->setObjectName("rackPreviewWidgetMSLabel");
    QLabel *statusLabel = new QLabel("PREVIEWING");
    statusLabel->setObjectName("rackPreviewWidgetStatusLabel");
    timeBoxLayout->addWidget(timeLabel, 0,0);
    timeBoxLayout->addWidget(msLabel,   0,1, Qt::AlignBottom);
    timeBoxLayout->addWidget(statusLabel, 1,0,1,2, Qt::AlignHCenter);
    RPushButton *play = new RPushButton;
    play->setObjectName("rackPreviewWidgetPlay");
    RPushButton *stop = new RPushButton;
    stop->setObjectName("rackPreviewWidgetStop");
    boxLeftLayout->addWidget(timeBox);
    boxLeftLayout->addWidget(play);
    boxLeftLayout->addWidget(stop);

    QWidget *boxRight = new QWidget;
    boxRight->setObjectName("rackPreviewWidgetBox");
    QGridLayout *boxRightLayout = new QGridLayout(boxRight);
    QLabel *jumpLabel = new QLabel(tr("Jump To"));
    jumpLabel->setObjectName("rackPreviewWidgetJumpLabel");
    RPushButton *startButton = new RPushButton(tr("Start"));
    startButton->setObjectName("rackPreviewWidgetJumpButton");
    RPushButton *secondsButton = new RPushButton(tr("Last 10s"));
    secondsButton->setObjectName("rackPreviewWidgetJumpButton");
    boxRightLayout->addWidget(jumpLabel,     0,0,1,2, Qt::AlignHCenter);
    boxRightLayout->addWidget(startButton,   1,0);
    boxRightLayout->addWidget(secondsButton, 1,1);

    QLabel *startLabel = new QLabel(tr("00:00"));
    startLabel->setObjectName("rackPreviewWidgetSliderLabels");
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setObjectName("rackPreviewWidgetSlider");
    QLabel *endLabel = new QLabel(tr("04:04"));
    endLabel->setObjectName("rackPreviewWidgetSliderLabels");
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(startLabel);
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(endLabel);

    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(18,18,18,18);
    layout->addWidget(label,        0,0,1,2);
    layout->addWidget(boxLeft,      1,0);
    layout->addWidget(boxRight,     1,1);
    layout->addLayout(sliderLayout, 2,0,1,2);


    setLayout(layout);

}

void RPreviewWidget::fade(bool in)
{
    raise();
    m_ani->stop();
    in ? m_ani->setStartValue(QPoint(x(), parentWidget()->height())) : m_ani->setStartValue(QPoint(x(), parentWidget()->height() - height() + 4));
    in ? m_ani->setEndValue(QPoint(x(), parentWidget()->height() - height() + 4)) : m_ani->setEndValue(QPoint(x(), parentWidget()->height()));
    m_ani->start();
    m_in = in;
}

void RPreviewWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool RPreviewWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == parent())
    {
        if (event->type() == QEvent::Resize) {

            //calc x position
            int x = (parentWidget()->width() - width()) / 2;
            QToolBar *mainToolBar = parentWidget()->findChild<QToolBar *>("rackMainToolBar");
            if (mainToolBar)
            {
                if (x <= mainToolBar->width() + 1) x = mainToolBar->width() + 2;
            }

            //calc y position
            int y = 0;
            m_in ? y = parentWidget()->height() - height() + 4 : y = parentWidget()->height();

            move(x, y);
        }
    }
    return QWidget::eventFilter(obj, event);
}



