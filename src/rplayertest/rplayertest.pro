TEMPLATE = app
TARGET = rplayertest
DEPENDPATH += .
INCLUDEPATH += ../rackclasses

# Input
HEADERS += mainwindow.h \ 
    ../rackclasses/rplayer.h

SOURCES += main.cpp \ 
    mainwindow.cpp \
    ../rackclasses/rplayer.cpp

# bass library:
unix:!macx: LIBS += -L$$PWD/../../bass/x64/ -lbass
LIBS += -L$$PWD/../../bass/ -lbass
INCLUDEPATH += $$PWD/../../bass
DEPENDPATH += $$PWD/../../bass
