/*
    Copyright (C) 2011, Steffen Müller and the r.a.c.k. team.
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

#ifndef RSPLITTER_H
#define RSPLITTER_H
#include <QSplitterHandle>
#include <QSplitter>


class RSplitter : public QSplitter
{
    Q_OBJECT

public:

    explicit RSplitter(Qt::Orientation orientation, QWidget *parent = 0);

signals:

    void enterSettingsMode();
    void leaveSettingsMode();

protected:

    QSplitterHandle *createHandle();

};



class RSplitterHandle : public QSplitterHandle
{
    Q_OBJECT

public:

    RSplitterHandle(Qt::Orientation orientation, RSplitter *parent);

public slots:

    void enterSettingsMode();
    void leaveSettingsMode();

protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:

    QColor m_handleColor;

};


#endif //RSPLITTER_H
