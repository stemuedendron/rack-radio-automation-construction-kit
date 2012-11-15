/*
    Copyright (C) 2012, Steffen Müller and the r.a.c.k. team.
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
#include <QtGui>
#include <QtWebKit>

RWebBrowser::RWebBrowser(ICore *api, QWidget *parent)
    : QMainWindow(parent),
      m_core(api),
      m_webView(new QWebView),
      m_locationEdit(new QLineEdit),
      m_progress(0)
{

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    //m_webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    m_locationEdit->setSizePolicy(QSizePolicy::Expanding, m_locationEdit->sizePolicy().verticalPolicy());

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(m_webView->pageAction(QWebPage::Back));
    toolBar->addAction(m_webView->pageAction(QWebPage::Forward));
    toolBar->addAction(m_webView->pageAction(QWebPage::Reload));
    toolBar->addAction(m_webView->pageAction(QWebPage::Stop));
    toolBar->addWidget(m_locationEdit);

    QObject::connect(m_webView, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    QObject::connect(m_webView, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    QObject::connect(m_webView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    QObject::connect(m_webView, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    QObject::connect(m_locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    m_webView->load(QUrl("http://www.radiofrei.de/"));
    setCentralWidget(m_webView);

    ///test zoom

    QSpinBox *sbox = new QSpinBox;
    sbox->setValue(100);
    sbox->setRange(30,200);
    sbox->setSingleStep(1);
    QObject::connect(sbox, SIGNAL(valueChanged(int)), SLOT(setZoom(int)));
    toolBar->addWidget(sbox);


    ///

    ////test overlay button

//    QWidget *testButton = new QWidget(m_webView);
//    testButton->setPalette(QPalette(QColor(0,0,0,160)));
//    testButton->setAutoFillBackground(true);
//    testButton->setGeometry(10,10,80,40);


    ///

}

void RWebBrowser::adjustLocation()
{
    m_locationEdit->setText(m_webView->url().toString());

    QWebFrame* frame = m_webView->page()->currentFrame();
    if (frame!=NULL)
    {
        QWebElementCollection collection = frame->findAllElements("a");
        foreach (QWebElement element, collection)
        {
            QStringList attributesList = element.attributeNames();
            foreach (QString attributeName, attributesList)
            {

                if (attributeName != "href") break;
                if (element.attribute(attributeName).contains(".mp1", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".mp2", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".mp3", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".mpg", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".mpa", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".wav", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".ogg", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".m3u", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".pls", Qt::CaseInsensitive) or
                    element.attribute(attributeName).contains(".xspf", Qt::CaseInsensitive))
                {

                    QUrl baseUrl(m_webView->url());
                    QUrl relativeUrl = QUrl::fromEncoded(element.attribute(attributeName).toUtf8(), QUrl::StrictMode);
                    QString streamName = baseUrl.resolved(relativeUrl).toString();

                    qDebug() << streamName;


                }
            }
        }

    }

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
}

void RWebBrowser::setZoom(int z)
{
    qreal f = z / 100.0;
    m_webView->setZoomFactor(f);
    qDebug() << f;
}
