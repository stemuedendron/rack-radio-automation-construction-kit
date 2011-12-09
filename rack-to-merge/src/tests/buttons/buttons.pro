TARGET = buttons

INCLUDEPATH += ../../rackclasses

HEADERS += mainwindow.h \ 
    ../../rackclasses/fscartbutton.h \
    ../../rackclasses/fsplaybutton.h \
    ../../rackclasses/fsclockbutton.h \
    ../../rackclasses/fsinterface.h \
    ../../rackclasses/rdesktop.h \
    ../../rackclasses/rsplitter.h \
    ../../rackclasses/rpluginhost.h \
    ../../rackclasses/fsgradbutton.h
SOURCES += main.cpp \ 
    mainwindow.cpp \
    ../../rackclasses/fsclockbutton.cpp \
    ../../rackclasses/fsplaybutton.cpp \
    ../../rackclasses/fscartbutton.cpp \
    ../../rackclasses/rdesktop.cpp \
    ../../rackclasses/rsplitter.cpp \
    ../../rackclasses/rpluginhost.cpp \
    ../../rackclasses/fsgradbutton.cpp

RESOURCES += ../../rack.qrc
