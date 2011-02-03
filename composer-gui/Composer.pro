# -------------------------------------------------
# Project created by QtCreator 2010-03-18T18:30:03
# -------------------------------------------------
TARGET = Composer
TEMPLATE = app
CONFIG += release
QT += xml
MOC_DIR = .mocs
OBJECTS_DIR = .objs

macx:ICON = images/Composer.icns

macx:LOCATION = /Library/Frameworks
else:unix:LOCATION = /usr/local
else:win32:LOCATION = C:/Composer

macx:INSTALLBASE = /Applications/Composer
else:unix:INSTALLBASE = /usr/local
win32:INSTALLBASE = C:/Composer

target.path = $$INSTALLBASE
INCLUDEPATH += include/

macx { 
    LIBS += -L/Library/Frameworks \
            -lqscintilla2 \
        -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += $$LOCATION/include/composer
}
else:win32 {
    LIBS += -L$$LOCATION/ \
        -lComposerCore1
    INCLUDEPATH += $$LOCATION/include/composer
}


SOURCES += main.cpp \
    src/MainWindow.cpp \
    src/wizard/ProjectWizard.cpp \
    src/wizard/ProjectEntryPage.cpp \
    src/wizard/DocumentWizard.cpp \
    src/wizard/DocumentEntryPage.cpp \
    src/PreferencesDialog.cpp \
    src/WorkspaceModel.cpp \
    src/ProjectTreeView.cpp \
    src/WorkspaceSwitch.cpp

HEADERS += include/MainWindow.h \
    include/wizard/ProjectWizard.h \
    include/wizard/ProjectEntryPage.h \
    include/wizard/DocumentWizard.h \
    include/wizard/DocumentEntryPage.h \
    include/PreferencesDialog.h \
    include/WorkspaceModel.h \
    include/ProjectTreeView.h \
    include/WorkspaceSwitch.h

RESOURCES += images.qrc

FORMS   += ui/PreferencesDialog.ui

INSTALLS += target
