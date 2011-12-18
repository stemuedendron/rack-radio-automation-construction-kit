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

#ifndef RDESKTOP_H
#define RDESKTOP_H
#include <QWidget>

class RSplitter;
class QXmlStreamWriter;


class RDesktop : public QWidget
 {
    Q_OBJECT

public:

    explicit RDesktop(QWidget *parent = 0);

private:

    void saveSplitter(RSplitter *splitter);
    void saveSplitterItem(QObject *obj, QXmlStreamWriter *xml);

    void saveSplittertoXML(RSplitter *splitter);
    void saveSplitterItemtoXML(QObject *obj, QXmlStreamWriter *xml);

private slots:

    void btNewRWidgetClick(int);
    void btCloseRWidgetClick();

public slots:

    void setConfigModus(bool);

    //debug this
    void saveRWidgets();

signals:

    void changeConfigModus(bool);

};

#endif // RDESKTOP_H






