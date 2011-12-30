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

#ifndef RACKWINDOW_H
#define RACKWINDOW_H

#include <QWidget>

//the api:
class CoreImpl;
class QSignalMapper;
class QSplitter;

class RackWindow : public QWidget
{
    Q_OBJECT

public:

    explicit RackWindow(QWidget *parent = 0);
    ~RackWindow();

signals:

public slots:

private slots:

    void createPluginHost(int);
    void closePluginHost(QWidget *pluginHost);
    void loadPlugin(QWidget *pluginHost);

signals:

    void setSettingsMode(int);

private:

    enum NewSplitterLocation {
        NewSplitterLeft = -1,
        NewSplitterRight = 1,
        NewSplitterTop = -2,
        NewSplitterBottom = 2
    };

    CoreImpl *m_coreImpl;
    QSignalMapper *m_mapperLoadNewPlugin;
    QSignalMapper *m_mapperclosePluginHost;
    QSplitter *m_mainSplitter;

};

#endif // RACKWINDOW_H
