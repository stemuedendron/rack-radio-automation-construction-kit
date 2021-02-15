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

#ifndef RWEBBROWSER_H
#define RWEBBROWSER_H

#include <QWidget>

class ICore;
class QVBoxLayout;
class QWebEngineView;
class QLineEdit;

class RWebBrowser : public QWidget
{
    Q_OBJECT

public:

    explicit RWebBrowser(ICore *api, QWidget *parent = 0);

protected slots:

    void adjustLocation();
    void changeLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void setZoom(int z);

private:

    ICore* m_core;
    QWebEngineView *m_webView;
    QLineEdit *m_locationEdit;
    QVBoxLayout *m_buttonLayout;
    int m_progress;
};

#endif // RWEBBROWSER_H
