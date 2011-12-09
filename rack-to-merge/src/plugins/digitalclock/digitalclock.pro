
TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH += ../../rackclasses

HEADERS         = \
                  ../../rackclasses/fsinterface.h \
    ../../rackclasses/fsgradbutton.h \
    ../../rackclasses/fscolordialog.h \
    fsdigitalclockplugin.h \
    fsdigitalclock.h


SOURCES         = \
    ../../rackclasses/fsgradbutton.cpp \
    ../../rackclasses/fscolordialog.cpp \
    fsdigitalclockplugin.cpp \
    fsdigitalclock.cpp


TARGET          = $$qtLibraryTarget(digitalclockplugin)



DESTDIR         = ../../coreapp/plugins
