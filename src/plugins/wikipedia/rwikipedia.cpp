/*
    Copyright (C) 2013, Steffen Müller.
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
#include "rwikipedia.h"
#include <QtWebKitWidgets>


RWikipedia::RWikipedia(ICore *api, QWidget *parent)
    : QWidget(parent),
      m_core(api)
{

    QWebView *webView = new QWebView;
    //make sure we use application wide NetworkAccessManager:
    webView->page()->setNetworkAccessManager(m_core->networkAccessManager());
    webView->load(QUrl("http://de.mobile.wikipedia.org/"));
    //webView->setZoomFactor(0.8);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(webView);
    setLayout(layout);

}
