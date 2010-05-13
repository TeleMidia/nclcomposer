# -------------------------------------------------
# Project created by QtCreator 2010-03-18T18:30:03
# -------------------------------------------------
TARGET = Composer
TEMPLATE = app
CONFIG += debug

macx::LOCATION = /Library/Frameworks
else:unix:LOCATION = /usr/local

debug:INSTALLBASE = ./debug
release {
    macx:INSTALLBASE = /Application/Composer
    else:unix:INSTALLBASE = /usr/local
    win32:INSTALLBASE = C:\Composer
}

macx { 
    LIBS += -framework \
        ComposerCoreModel \
        -framework \
        ComposerCoreControl
    INCLUDEPATH += /Library/Frameworks/ComposerCoreModel.framework/ \
        /Library/Frameworks/ComposerCoreControl.framework/
}
else:unix { 
    LIBS += -lComposerCoreModel \
        -lComposerCoreControl
    INCLUDEPATH += $$LOCATION/include/model \
        $$LOCATION/include/control
}
SOURCES += main.cpp \
    src/gui/MainWindow.cpp \
    src/gui/ProjectWizard.cpp \
    src/manager/CoreManager.cpp
HEADERS_GUI += include/gui/MainWindow.h \
    include/gui/ProjectWizard.h 

HEADERS_MANAGER +=  include/manager/CoreManager.h
RESOURCES += images.qrc

HEADERS += $$HEADERS_GUI $$HEADERS_MANAGER


