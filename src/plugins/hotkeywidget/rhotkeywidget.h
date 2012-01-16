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

#ifndef RHOTKEYWIDGET_H
#define RHOTKEYWIDGET_H

#include "rblinkbutton.h"

#include <QWidget>

class ICore;
class QStringList;
class QStackedWidget;
class QLabel;
class QVBoxLayout;


class RHotKeyWidget : public QWidget
{
    Q_OBJECT

public:

    explicit RHotKeyWidget(QWidget *parent = 0, ICore *api = 0);

signals:

    void enableNPButtons(bool);
    void enableIndexButton(bool);
    void editMode(bool);

public slots:

private slots:

    void createHotkeyPage(QString title, int rows, int cols);
    void showIndexPage();
    void showPreviousPage();
    void showNextPage();
    void indexPageClicked();
    void hotkeyStackChanged(int);

private:

    ICore* m_core;
    int sortedInsert(const QString &title);
    void createHotKeys(int index, int rows, int cols);
    void createIndexButton(int index, const QString &title, int keys);
    void createEditModeLabel(QWidget *widget);
    void editHotkeyPage();
    RBlinkButton *m_btEdit;
    QStringList m_pageList;
    QStackedWidget *m_layout;
    QStackedWidget *m_hotkeyStack;
    QLabel *m_pageTitle;
    QVBoxLayout *m_indexPageLayout;

};

#endif // RHOTKEYWIDGET_H
