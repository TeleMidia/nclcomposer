TARGET = composer
TEMPLATE = app
CONFIG += qt warn_on debug console
QT += xml webkit network

RC_FILE = images/nclcomposer.rc

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

unix {
    target.path = $$INSTALLBASE/bin

    QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN/../lib/composer\'
    QMAKE_LFLAGS += --rpath=\'\$\$ORIGIN/../lib/composer/extension\'
}
else {
    taget.path = $$INSTALLBASE
}

INCLUDEPATH += include/

macx {
    LIBS += \
        -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
}
else:unix {
    LIBS += -L$$LOCATION/lib/composer -lComposerCore
    INCLUDEPATH +=  $$LOCATION/include/composer \
                    $$LOCATION/include/composer/core
}
else:win32 {
    LIBS += -L$$LOCATION -lComposerCore1
    INCLUDEPATH += $$LOCATION/include/composer \
                   $$LOCATION/include/composer/core
}

SOURCES += main.cpp \
    src/ComposerMainWindow.cpp \
    src/wizard/ProjectWizard.cpp \
    src/wizard/ProjectEntryPage.cpp \
    src/wizard/DocumentWizard.cpp \
    src/wizard/DocumentEntryPage.cpp \
    src/PreferencesDialog.cpp \
    src/PerspectiveManager.cpp \
    src/PluginDetailsDialog.cpp \
    src/EnvironmentPreferencesWidget.cpp \
    src/WelcomeWidget.cpp

HEADERS += include/ComposerMainWindow.h \
    include/wizard/ProjectWizard.h \
    include/wizard/ProjectEntryPage.h \
    include/wizard/DocumentWizard.h \
    include/wizard/DocumentEntryPage.h \
    include/PreferencesDialog.h \
    include/PerspectiveManager.h \
    include/PluginDetailsDialog.h \
    include/EnvironmentPreferencesWidget.h \
    include/IPreferencePage.h \
    include/WelcomeWidget.h

RESOURCES += images.qrc

FORMS   += ui/PreferencesDialog.ui \
    ui/ComposerMainWindow.ui \
    ui/PerspectiveManager.ui \
    ui/RunGingaConfig.ui \
    ui/PluginDetailsDialog.ui \
    ui/EnvironmentPreferencesWidget.ui \
    ui/WelcomeWidget.ui

INSTALLS += target
