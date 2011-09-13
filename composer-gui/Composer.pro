TARGET = composer
TEMPLATE = app
CONFIG += qt warn_on debug console
QT += xml webkit network

#DEFINES += USE_MDI

RC_FILE = images/nclcomposer.rc

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx {
  INSTALLBASE = /Applications/Composer
  ICON =  images/Composer.icns
}
else:unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  } 
  INSTALLBASE = $$PREFIX
  
  DATADIR = $$PREFIX/share

  # set the path to install desktop configuration
  desktop.path = $$DATADIR/applications/
  desktop.files = $${TARGET}.desktop

  icon64.path = $$DATADIR/icons/gnome/64x64/apps
  icon64.files = images/$${TARGET}.png

  icon48.path = $$DATADIR/icons/gnome/48x48/apps
  icon48.files = images/$${TARGET}.png
}
else win32 {
  INSTALLBASE = C:/Composer
}

DEFINES += EXT_DEFAULT_PATH=\"\\\"$$PREFIX\\\"\"

unix {
    target.path = $$INSTALLBASE/bin

    QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN/../lib/composer\'
    QMAKE_LFLAGS += --rpath=\'\$\$ORIGIN/../lib/composer/extensions\'
}
else {
    taget.path = $$INSTALLBASE
}

INCLUDEPATH += include/

macx {
    LIBS += -framework ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
}
else:unix {
    LIBS += -L$$INSTALLBASE/lib/composer -lComposerCore
    INCLUDEPATH +=  $$INSTALLBASE/include/composer \
                    $$INSTALLBASE/include/composer/core
}
else:win32 {
    LIBS += -L$$INSTALLBASE -lComposerCore1
    INCLUDEPATH += $INSTALLBASE/include/composer \
                   $$INSTALLBASE/include/composer/core
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

INSTALLS += target desktop icon64 icon48
