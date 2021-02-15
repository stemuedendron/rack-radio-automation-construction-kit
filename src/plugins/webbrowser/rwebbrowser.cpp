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

#include "icore.h"
#include "rwebbrowser.h"
#include <QtWidgets>
#include <QtWebEngineWidgets>

RWebBrowser::RWebBrowser(ICore *api, QWidget *parent)
    : QWidget(parent),
      m_core(api),
      m_webView(new QWebEngineView),
      m_locationEdit(new QLineEdit),
      m_buttonLayout(new QVBoxLayout),
      m_progress(0)
{



    //TODO: make this configurable via settings
    //m_webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

    m_locationEdit->setSizePolicy(QSizePolicy::Expanding, m_locationEdit->sizePolicy().verticalPolicy());

    QToolBar *toolBar = new QToolBar;

    toolBar->addAction(m_webView->pageAction(QWebEnginePage::Back));
    toolBar->addAction(m_webView->pageAction(QWebEnginePage::Forward));
    toolBar->addAction(m_webView->pageAction(QWebEnginePage::Reload));
    toolBar->addAction(m_webView->pageAction(QWebEnginePage::Stop));
    toolBar->addWidget(m_locationEdit);

    QObject::connect(m_webView, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    QObject::connect(m_webView, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    QObject::connect(m_webView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    QObject::connect(m_webView, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    QObject::connect(m_locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));



    //TODO: load webpage given in user settings
    m_webView->load(QUrl("http://www.radiofrei.de/"));




    ///test zoom

    QSpinBox *sbox = new QSpinBox;
    sbox->setValue(100);
    sbox->setRange(30,200);
    sbox->setSingleStep(1);
    QObject::connect(sbox, SIGNAL(valueChanged(int)), SLOT(setZoom(int)));
    toolBar->addWidget(sbox);


    ///


    //create webView layout:
    QVBoxLayout *webLayout = new QVBoxLayout;
    webLayout->setSpacing(3);
    webLayout->setContentsMargins(0,0,0,0);
    webLayout->addWidget(toolBar);
    webLayout->addWidget(m_webView);

    //create main layout:
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(3);
    layout->setContentsMargins(0,0,0,0);
    layout->addLayout(webLayout);
    layout->addLayout(m_buttonLayout);

    setLayout(layout);




}

void RWebBrowser::adjustLocation()
{
    m_locationEdit->setText(m_webView->url().toString());
}

void RWebBrowser::changeLocation()
{
    QUrl baseUrl("http://");
    QUrl url = baseUrl.resolved(QUrl(m_locationEdit->text()));
    m_webView->load(url);
    m_webView->setFocus();
}

void RWebBrowser::adjustTitle()
{
//    if (m_progress <= 0 || m_progress >= 100)
//        setWindowTitle(m_webView->title());
//    else
//        setWindowTitle(QString("%1 (%2%)").arg(m_webView->title()).arg(m_progress));
}

void RWebBrowser::setProgress(int p)
{
//    m_progress = p;
//    adjustTitle();
}

void RWebBrowser::finishLoading(bool)
{
//    m_progress = 100;
    //    adjustTitle();

//    QWebFrame* frame = m_webView->page()->mainFrame();
//    if (frame!=NULL)
//    {
//        QWebElementCollection elements = frame->findAllElements("a");
//        foreach (QWebElement element, elements)
//        {
//            QStringList attributesList = element.attributeNames();
//            foreach (QString attributeName, attributesList)
//            {
//                if (attributeName != "href") break;
//                QRegExp rx(".*.mp1$|.*.mp2$|.*.mp3$|.*.mp4$|.*.mpg$|.*.mpa$|.*.wav$|.*.ogg$|.*.m3u$|.*.pls$|.*.xspf$");
//                rx.setCaseSensitivity(Qt::CaseInsensitive);
//                if (element.attribute(attributeName).contains(rx))
//                {

//                    QUrl baseUrl(m_webView->url());
//                    QUrl relativeUrl = QUrl::fromEncoded(element.attribute(attributeName).toUtf8(), QUrl::StrictMode);
//                    QString streamName = baseUrl.resolved(relativeUrl).toString();

//                    qDebug() << streamName;

//                    //test button

//                    QPushButton *testButton = new QPushButton(streamName);
//                    m_buttonLayout->addWidget(testButton);

//                    //


//                }
//            }
//        }

//    }
}

void RWebBrowser::setZoom(int z)
{
    qreal f = z / 100.0;
    m_webView->setZoomFactor(f);
    qDebug() << f;
}
