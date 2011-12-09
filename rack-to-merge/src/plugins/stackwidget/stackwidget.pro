
TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH += ../../rackclasses

HEADERS         = stackwidgetplugin.h \
    ../../rackclasses/fsinterface.h \
    fsstackbutton.h \
    ../../rackclasses/fsgradbutton.h \
    fsstackwidget.h \
    ../../rackclasses/fsplaybutton.h \
    ../../rackclasses/fsclockbutton.h \
    ../../rackclasses/fscartbutton.h

SOURCES         = stackwidgetplugin.cpp \
    fsstackbutton.cpp \
    ../../rackclasses/fsgradbutton.cpp \
    fsstackwidget.cpp \
    ../../rackclasses/fsplaybutton.cpp \
    ../../rackclasses/fsclockbutton.cpp \
    ../../rackclasses/fscartbutton.cpp


TARGET          = $$qtLibraryTarget(stackwidgetplugin)



DESTDIR         = ../../coreapp/plugins
