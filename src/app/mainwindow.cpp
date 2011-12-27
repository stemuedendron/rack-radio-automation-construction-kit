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

#include "mainwindow.h"
#include "coreimpl.h"
#include "rdesktop.h"

#include <QtGui>


MainWindow::MainWindow() :
    //create the rack api object:
    m_coreImpl(new CoreImpl(this)),
    m_desktop(0),
    m_taskbar(0)
{

    setWindowTitle("R.A.C.K.");

    QFile file(":/stylesheets/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);

    m_desktop = new RDesktop();
    m_taskbar = new QWidget();

    m_settingsButton = new QPushButton("Settings");
    m_settingsButton->setCheckable(true);
    QObject::connect(m_settingsButton, SIGNAL(toggled(bool)), m_desktop, SLOT(setConfigModus(bool)));

    QPushButton *btQuit = new QPushButton("Quit");
    QObject::connect(btQuit,SIGNAL(clicked()),SLOT(close()));

    m_saveButton = new QPushButton("Save");
    QObject::connect(m_saveButton, SIGNAL(clicked()), m_desktop, SLOT(savePluginHosts()));


    QHBoxLayout *barLayout = new QHBoxLayout();
    barLayout->addWidget(m_settingsButton);
    barLayout->addWidget(btQuit);

    barLayout->addWidget(m_saveButton);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_desktop);

    layout->addLayout(barLayout);

    layout->addWidget(m_taskbar);

    setLayout(layout);

}

MainWindow::~MainWindow()
{
    delete m_coreImpl;
    m_coreImpl = 0;
}

