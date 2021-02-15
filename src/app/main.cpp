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

    app.setApplicationName("R.A.C.K.");
    app.setApplicationDisplayName("R.A.C.K - Radio Automation Construction Kit");
    app.setOrganizationName("Radio F.R.E.I.");
    app.setOrganizationDomain("rack-broadcast.org");
    app.setApplicationVersion(RACK_VERSION_STR);

    // logging:
    using namespace QsLogging;
    Logger& logger = Logger::instance();

    //TODO: set standard loglevel from user settings:
    logger.setLoggingLevel(QsLogging::TraceLevel);

    const QString logPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    QDir logDir;
    logDir.mkpath(logPath);
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(logPath + "/rack.log", EnableLogRotation, MaxSizeBytes(1024*1000), MaxOldLogCount(10)));
    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(fileDestination);
    logger.addDestination(debugDestination);


    QLOG_INFO() << "start R.A.C.K., Version" << RACK_VERSION_STR;
    QLOG_INFO() << "Built on" << RACK_BUILD_DATE_STR << "at" << RACK_BUILD_TIME_STR;
    QLOG_INFO() << "Built with Qt" << QT_VERSION_STR << "running on" << qVersion();


    //connect to database:
    QLOG_INFO() << "available database drivers:";
    QStringList driverList;
    driverList = QSqlDatabase::drivers();
    foreach (QString driver, driverList) {
        QLOG_INFO() << qPrintable(driver);
    }


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
        QLOG_ERROR() << "no database connection";
        QLOG_ERROR() << db.lastError();
    }
    else
    {
        QLOG_INFO() << "database connected";
    }

    RackWindow mainWindow;
    mainWindow.resize(800, 600);
//  mainWindow.setWindowState(Qt::WindowMaximized);
    mainWindow.show();
    return app.exec();
}
