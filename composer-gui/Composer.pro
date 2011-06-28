# -------------------------------------------------
# Project created by QtCreator 2010-03-18T18:30:03
# -------------------------------------------------
TARGET = Composer
TEMPLATE = app
CONFIG += debug console
QT += xml webkit

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

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
    LIBS += \
        -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH +=  $$LOCATION/include/composer \
                    $$LOCATION/include/composer/core
}
else:win32 {
    LIBS += -L$$LOCATION/ \
        -lComposerCore1
    INCLUDEPATH += $$LOCATION/include/composer
}


SOURCES += main.cpp \
    src/ComposerMainWindow.cpp \
    src/wizard/ProjectWizard.cpp \
    src/wizard/ProjectEntryPage.cpp \
    src/wizard/DocumentWizard.cpp \
    src/wizard/DocumentEntryPage.cpp \
    src/PreferencesDialog.cpp \
    src/PerspectiveManager.cpp
    # src/ProjectTreeView.cpp
    # src/WorkspaceModel.cpp
    # src/WorkspaceSwitch.cpp

HEADERS += include/ComposerMainWindow.h \
    include/wizard/ProjectWizard.h \
    include/wizard/ProjectEntryPage.h \
    include/wizard/DocumentWizard.h \
    include/wizard/DocumentEntryPage.h \
    include/PreferencesDialog.h \
    include/PerspectiveManager.h
    # include/ProjectTreeView.h
    # include/WorkspaceModel.h
    # include/WorkspaceSwitch.h

RESOURCES += images.qrc

FORMS   += ui/PreferencesDialog.ui \
    ui/ComposerMainWindow.ui \
    ui/PerspectiveManager.ui \
    ui/RunGingaConfig.ui

INSTALLS += target
