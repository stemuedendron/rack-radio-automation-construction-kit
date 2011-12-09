
TEMPLATE        = lib
CONFIG         += plugin
INCLUDEPATH += ../../rackclasses

HEADERS         = fsradioclockplugin.h \
                  ../../rackclasses/fsinterface.h \
                  fsradioclock.h \
    ../../rackclasses/fsgradbutton.h \
    ../../rackclasses/fscolordialog.h


SOURCES         = fsradioclockplugin.cpp \
                  fsradioclock.cpp \ 
    ../../rackclasses/fsgradbutton.cpp \
    ../../rackclasses/fscolordialog.cpp


TARGET          = $$qtLibraryTarget(radioclockplugin)



DESTDIR         = ../../coreapp/plugins
