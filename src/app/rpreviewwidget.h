/*
    Copyright (C) 2012, Steffen Müller and the r.a.c.k. team.
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

#ifndef RPREVIEWWIDGET_H
#define RPREVIEWWIDGET_H

#include <QWidget>

class QPropertyAnimation;

class RPreviewWidget : public QWidget
{
    Q_OBJECT

public:

    explicit RPreviewWidget(QWidget *parent = 0);
    
signals:
    
public slots:

    void fade(bool in);

protected:

    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *obj, QEvent *event);

private:

    QPropertyAnimation *m_ani;
    bool m_in;

};

#endif // RPREVIEWWIDGET_H
