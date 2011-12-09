TARGET = colordialog

INCLUDEPATH += ../../rackclasses


HEADERS += mainwindow.h \ 
    ../../rackclasses/fscolordialog.h \
    ../../rackclasses/fsgradbutton.h
SOURCES += main.cpp \ 
    mainwindow.cpp \
    ../../rackclasses/fscolordialog.cpp \
    ../../rackclasses/fsgradbutton.cpp

RESOURCES += ../../rack.qrc
