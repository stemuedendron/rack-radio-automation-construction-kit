#include <QApplication>
#include "mainwindow.h"

int main( int argc, char **argv )
{
    QApplication myapp( argc, argv );
    MainWindow w;
    w.show();
    return myapp.exec();
}



