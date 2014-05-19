/*
    Copyright (C) 2011, Steffen Müller.
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
#include "rackwindow.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include <QtWidgets>
#include <QtSql>


int main(int argc, char *argv[])
{

    //TODO: read carefully QApplication and Qt Namespace docs!!!!

    //QApplication::setStyle(QStyleFactory::create(QLatin1String("windows")));

    QApplication app(argc, argv);
    using namespace QsLogging;



    // 1. init the logging mechanism
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(app.applicationDirPath()).filePath("log.txt"));

    // 2. add two destinations
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
                                      sLogPath, EnableLogRotation, MaxSizeBytes(512), MaxOldLogCount(2)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);
    logger.addDestination(fileDestination);


    // 3. start logging
//    QLOG_INFO() << "Program started";
//    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();

//    QLOG_TRACE() << "Here's a" << QString::fromUtf8("trace") << "message";
//    QLOG_DEBUG() << "Here's a" << static_cast<int>(QsLogging::DebugLevel) << "message";
//    QLOG_WARN()  << "Uh-oh!";
//    qDebug() << "This message won't be picked up by the logger";
//    QLOG_ERROR() << "An error has occurred";
//    qWarning() << "Neither will this one";
//    QLOG_FATAL() << "Fatal error!";


    app.setApplicationName("r.a.c.k.");
    app.setApplicationDisplayName("r.a.c.k. - Radio Automation Construction Kit");
    app.setOrganizationName("Radio F.R.E.I.");
    app.setOrganizationDomain("rack-broadcast.org");
    app.setApplicationVersion(RACK_VERSION_STR);


    qDebug() << "welcome to r.a.c.k." << RACK_VERSION_STR << "build:" << __TIME__ << __DATE__;
    qDebug() << "git:" << RACK_VERSION_GIT;



    //connect to database:
    //TODO: settings from config
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("rack");
    db.setUserName("postgres");
    db.setPassword("");
    if (!db.open())
    {
//        QMessageBox::critical(0, qApp->tr("Cannot open database"),
//                              qApp->tr("Unable to establish a database connection.\n"
//                                       "Click Cancel to exit."), QMessageBox::Cancel);
//        return 1;
        qDebug() << "no database connection";
        qDebug() << db.lastError();
    }

    QStringList driverList;
    driverList = QSqlDatabase::drivers();
    foreach (QString driver, driverList) {
        qDebug() << driver;
    }



    RackWindow mainWindow;

    mainWindow.resize(800, 600);
//  mainWindow.setWindowState(Qt::WindowMaximized);

    mainWindow.show();

    return app.exec();
}
