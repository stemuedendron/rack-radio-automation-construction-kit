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

#ifndef RPLUGINHOST_H
#define RPLUGINHOST_H

#include "rinterface.h"
#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QSignalMapper;

class RPluginHost : public QWidget
{
    Q_OBJECT

public:

    explicit RPluginHost(QWidget *parent = 0);

signals:

    void btNewWidgetCLick(int);
    void btCloseWidgetClick();

    void pluginAdded();

    void setPluginsVisible(bool);

protected:

    //we reimplemented the resizeEvent() handler for resizising the layoutconfig widget
    void resizeEvent(QResizeEvent *ev);

public slots:

    void setLayoutVisible(bool);

    void newPlugin();

    void removePlugin(int);
    void showPlugin(int);

private:

    //widgets for settings
    //FSLayoutWidget *m_layoutWidget;
    QWidget *m_wgSettings;


    QVBoxLayout *m_vlMiddle;
    QPushButton *m_btLeft;
    QPushButton *m_btRight;
    QPushButton *m_btTop;
    QPushButton *m_btBottom;
    QPushButton *m_btClose;

    QPushButton *m_btMiddle;
    QSignalMapper *m_signalMapper;


    bool loadPlugin();

    QWidgetList m_wlPluginWidgets;

    RInterface *m_rinterface;
    QHBoxLayout *m_hlMain;

};

#endif // RPLUGINHOST_H
