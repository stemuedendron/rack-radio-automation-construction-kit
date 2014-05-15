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

#ifndef RCOUCHDBMODEL_H
#define RCOUCHDBMODEL_H

#include <QStandardItemModel>

class QNetworkAccessManager;
class QNetworkReply;

class RCouchDBModel : public QStandardItemModel
{

    Q_OBJECT

public:

    explicit RCouchDBModel(QNetworkAccessManager *nam, QUrl url, QObject *parent = 0);

signals:

public slots:

private slots:

    void finished();

private:

    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;

};

#endif // RCOUCHDBMODEL_H
