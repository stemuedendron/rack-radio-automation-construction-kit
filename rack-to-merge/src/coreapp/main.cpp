#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication::setGraphicsSystem("raster");

    QApplication app(argc, argv);
    RMainWindow mainWindow;
    mainWindow.resize(800, 600);
    mainWindow.setWindowState(Qt::WindowMaximized);
    //widget.setWindowState(Qt::WindowFullScreen);
    mainWindow.show();
    return app.exec();
}
