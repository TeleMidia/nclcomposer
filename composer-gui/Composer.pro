# -------------------------------------------------
# Project created by QtCreator 2010-03-18T18:30:03
# -------------------------------------------------
TARGET = Composer
TEMPLATE = app
CONFIG += debug
QT += xml
MOC_DIR = mocs
OBJECTS_DIR = objs

macx:LOCATION = /Library/Frameworks
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
    LIBS += -L/usr/local/lib/composer -lComposerCoreModel \
        -lComposerCoreControl
    INCLUDEPATH += $$LOCATION/include/composer/model \
        $$LOCATION/include/composer/control \
        $$LOCATION/include/composer

}
SOURCES += main.cpp \
    src/gui/MainWindow.cpp \
    src/gui/ProjectTreeWidget.cpp \
    src/gui/wizard/ProjectWizard.cpp \
    src/gui/wizard/ProjectEntryPage.cpp \
    src/gui/wizard/DocumentWizard.cpp \
    src/gui/wizard/DocumentEntryPage.cpp

HEADERS_GUI += include/gui/MainWindow.h \
    include/gui/ProjectTreeWidget.h \
    include/gui/wizard/ProjectWizard.h \
    include/gui/wizard/ProjectEntryPage.h \
    include/gui/wizard/DocumentWizard.h \
    include/gui/wizard/DocumentEntryPage.h

HEADERS_MANAGER +=
RESOURCES += images.qrc

HEADERS += $$HEADERS_GUI $$HEADERS_MANAGER
