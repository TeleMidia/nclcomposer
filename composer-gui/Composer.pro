# -------------------------------------------------
# Project created by QtCreator 2010-03-18T18:30:03
# -------------------------------------------------
TARGET = Composer
TEMPLATE = app
CONFIG += debug
QT += xml
MOC_DIR = mocs
OBJECTS_DIR = objs

macx:ICON = images/Composer.icns

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
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
}
else:unix { 
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += $$LOCATION/include/composer

}
SOURCES += main.cpp \
    src/MainWindow.cpp \
    src/ProjectTreeWidget.cpp \
    src/wizard/ProjectWizard.cpp \
    src/wizard/ProjectEntryPage.cpp \
    src/wizard/DocumentWizard.cpp \
    src/wizard/DocumentEntryPage.cpp \
    src/PreferencesDialog.cpp

HEADERS += include/MainWindow.h \
    include/ProjectTreeWidget.h \
    include/wizard/ProjectWizard.h \
    include/wizard/ProjectEntryPage.h \
    include/wizard/DocumentWizard.h \
    include/wizard/DocumentEntryPage.h \
    include/PreferencesDialog.h

RESOURCES += images.qrc

#HEADERS += $$HEADERS_GUI

FORMS   += ui/PreferencesDialog.ui
