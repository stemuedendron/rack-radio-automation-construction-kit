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

#include <QMainWindow>

class CoreImpl;
class RSplitter;
class QSignalMapper;
class QXmlStreamWriter;

class RackWindow : public QMainWindow
{
    Q_OBJECT

public:

    RackWindow();
    ~RackWindow();

public slots:

    //old save stuff
    //debug this
    void savePluginHosts();
    /////////////////////////

private slots:

    void createPluginHost(int);
    void closePluginHost(QWidget *pluginHost);
    void loadPlugin(QWidget *pluginHost);

    void toggleFullscreen();

signals:

    void enterSettingsMode();
    void leaveSettingsMode();

private:

    enum NewSplitterLocation {
        NewSplitterLeft = -1,
        NewSplitterRight = 1,
        NewSplitterTop = -2,
        NewSplitterBottom = 2
    };

    void createToolBars();

    //old save stuff
    void saveSplitter(RSplitter *splitter);
    void saveSplitterItem(QObject *obj, QXmlStreamWriter *xml);

    void saveSplittertoXML(RSplitter *splitter);
    void saveSplitterItemtoXML(QObject *obj, QXmlStreamWriter *xml);
    /////////////////////////////

    CoreImpl *m_coreImpl;
    RSplitter *m_mainSplitter;
    QSignalMapper *m_mapperLoadNewPlugin;
    QSignalMapper *m_mapperClosePluginHost;

};

#endif // RACKWINDOW_H
