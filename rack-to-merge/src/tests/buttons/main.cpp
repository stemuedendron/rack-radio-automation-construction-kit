#include <QApplication>
#include "mainwindow.h"

int main( int argc, char **argv )
{

    QApplication::setGraphicsSystem("raster");

    QApplication myapp( argc, argv );
    MainWindow w;
    w.show();
    return myapp.exec();
}



