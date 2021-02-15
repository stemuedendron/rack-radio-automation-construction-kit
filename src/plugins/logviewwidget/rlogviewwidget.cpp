/*
    Copyright (C) 2014, Steffen Müller.
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

#include "icore.h"
#include "rlogviewwidget.h"
#include "rtail.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QStandardPaths>

RLogViewWidget::RLogViewWidget(ICore *api, QWidget *parent) :
    QWidget(parent),
    m_core(api)
{

    QTextEdit *logViewer = new QTextEdit;
    logViewer->setObjectName("rackLogViewerTextEdit");
    logViewer->setReadOnly(true);

    const QString logPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    RTail *tail = new RTail(logPath + "/rack.log", this);
    tail->connect(tail, SIGNAL(logString(QString)), logViewer, SLOT(append(QString)));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(logViewer);
    setLayout(layout);

    m_core->logInfo("Log Viewer Plugin loaded");
}
