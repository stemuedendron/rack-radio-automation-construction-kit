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

#include "rcouchdbmodel.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


RCouchDBModel::RCouchDBModel(QNetworkAccessManager *nam, QUrl url, QObject *parent) :
    QStandardItemModel(parent),
    m_nam(nam)
{

    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply *reply = m_nam->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(populateFinished()));

}

void RCouchDBModel::populateFinished()
{

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    //qDebug() << doc.toJson();

    // check if reply is not empty
    if (!doc.isNull())
    {
        QJsonObject json = doc.object();
        // check if we have rows array in reply
        if (json["rows"].isArray())
        {
            QJsonArray rows = json["rows"].toArray();
            for (int i = 0; i < rows.size(); ++i)
            {
                QJsonObject row = rows[i].toObject();
                QJsonObject key = row["key"].toObject();

                setItem(i, 0 , new QStandardItem(QString(key["_id"].toString())));
                setItem(i, 1 , new QStandardItem(QString(key["artist"].toString())));
                setItem(i, 2 , new QStandardItem(QString(key["title"].toString())));

            }
        }
    }

    qDebug() << this->rowCount();

    reply->deleteLater();
    
    
}

void RCouchDBModel::couchDBChanged()
{
    
}



