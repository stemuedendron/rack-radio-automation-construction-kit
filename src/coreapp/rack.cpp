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

#include "rack.h"
#include "rdesktop.h"

#include <QtGui>

static Rack *m_instance = 0;

Rack *Rack::instance()
{
    return m_instance;
}

Rack::Rack(QWidget *parent) : QWidget(parent)
{
    m_instance = this;

    setWindowTitle("R.A.C.K.");

    QFile file(":/qss/default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    setStyleSheet(styleSheet);


    m_desktop = new RDesktop;

    m_btSettings = new QPushButton("Settings");
    m_btSettings->setCheckable(true);
    QObject::connect(m_btSettings, SIGNAL(toggled(bool)), m_desktop, SLOT(setConfigModus(bool)));

    QPushButton *btQuit = new QPushButton("Quit");
    QObject::connect(btQuit,SIGNAL(clicked()),SLOT(close()));

    m_btSavetest = new QPushButton("Save");
    QObject::connect(m_btSavetest, SIGNAL(clicked()), m_desktop, SLOT(savePluginHosts()));


    QHBoxLayout *barLayout = new QHBoxLayout;
    barLayout->addWidget(m_btSettings);
    barLayout->addWidget(btQuit);

    barLayout->addWidget(m_btSavetest);



    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(m_desktop);
    layout->addLayout(barLayout);
    setLayout(layout);

    startTimer(1000);


}

void Rack::timerEvent(QTimerEvent *)
 {
     emit timeStrChanged(QTime::currentTime().toString(tr("hh:mm:ss")));
     emit dateStrChanged(QDate::currentDate().toString(tr("dd.MM.yy")));
 }
