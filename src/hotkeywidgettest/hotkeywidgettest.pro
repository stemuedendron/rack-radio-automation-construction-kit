TEMPLATE = app
TARGET = hotkeywidgettest

HEADERS += mainwindow.h \  
    rhotkeys.h \
    rpageedit.h \
    rindexbutton.h \
    ../rackclasses/rbutton.h \
    ../rackclasses/rblinkbutton.h \
    ../rackclasses/rplayer.h

SOURCES += main.cpp \ 
    mainwindow.cpp \
    rhotkeys.cpp \
    rpageedit.cpp \
    rindexbutton.cpp \
    ../rackclasses/rbutton.cpp \
    ../rackclasses/rblinkbutton.cpp \
    ../rackclasses/rplayer.cpp

RESOURCES += \
    ../rack.qrc

OTHER_FILES += \
    ../../qss/default.qss \
    ../../qss/matrix.qss \
    ../../qss/default-old.qss

INCLUDEPATH += ../rackclasses


# bass library:
unix:!macx: LIBS += -L$$PWD/../../bass/x64/ -lbass
LIBS += -L$$PWD/../../bass/ -lbass
INCLUDEPATH += $$PWD/../../bass
DEPENDPATH += $$PWD/../../bass
