TARGET = composer
TEMPLATE = app

# I'm using FORCERELEASE variable because CONFIG and SUBDIR force three
# executions if qmake and the last one does not preserves CONFIG from command
# line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("Composer.pro RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("Composer.pro DEBUG build!")
}

#WHAT FEATURES TO COMPILE?
#CONFIG += clubencl
#DEFINES += USE_MDI

contains(RUNSSHON, true) {
  CONFIG += runssh_on
}


CONFIG += help
QT += core xml network webkit

#VERSION INFORMATION
GIT_VERSION=true
isEmpty(CPRVERSION) {
  #GUI_VERSION=$$system(git describe --tag | sed "s/v\(.*\)-.*-.*/\1/")
  GUI_VERSION=$$system(git describe --tag | sed "s/v//")
}
else {
  GUI_VERSION=$$CPRVERSION
  GIT_VERSION=false
}

message("NCL Composer GUI build version $${GUI_VERSION} (from git=$${GIT_VERSION})")

VERSTR = '\\"$${GUI_VERSION}\\"'
DEFINES += NCLCOMPOSER_GUI_VERSION=\"$${VERSTR}\"
DEFINES += BUILD_DATE=\"\\\"$$system(echo \"$${_DATE_}\" | sed \"s/ /\\\\\\ /g\")\"\\\"
#DEFINES += WITH_TEST_VERSION_MESSAGE=\"\\\"1\\\"\"

#NOTIFY SYSTEM
DEFINES += DEFAULT_MAX_NOTIFY_MESSAGES=\"4\"
DEFINES += DEFAULT_MIN_MESSAGE_ID_TO_SHOW=\"2\"
DEFINES += NCL_COMPOSER_NOTIFY_URL=\"\\\"http://composer.telemidia.puc-rio.br/update/CURRENT_VERSION\\\"\"

RC_FILE = images/nclcomposer.rc

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx {
  TARGET = Composer
  INSTALLBASE = /Applications
  ICON =  images/Composer.icns

  data.path = "/Library/Application Support/Composer/Data/"
}
else:unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  INSTALLBASE = $$PREFIX

  DATADIR = $$PREFIX/share

  # set the path to install desktop configuration
  desktop.path = $$DATADIR/applications/
  desktop.files = data/$${TARGET}.desktop

  icon256.files = images/256x256/$${TARGET}.png
  icon256.path = $$DATADIR/icons/gnome/256x256/apps
  
  icon128.files = images/128x128/$${TARGET}.png
  icon128.path = $$DATADIR/icons/gnome/128x128/apps

  icon64.files = images/64x64/$${TARGET}.png
  icon64.path = $$DATADIR/icons/gnome/64x64/apps

  icon48.path = $$DATADIR/icons/gnome/48x48/apps
  icon48.files = images/48x48/$${TARGET}.png

  icon32.path = $$DATADIR/icons/gnome/32x32/apps
  icon32.files = images/32x32/$${TARGET}.png

  icon24.path = $$DATADIR/icons/gnome/24x24/apps
  icon24.files = images/24x24/$${TARGET}.png

  icon22.path = $$DATADIR/icons/gnome/22x22/apps
  icon22.files = images/22x22/$${TARGET}.png

  icon16.path = $$DATADIR/icons/gnome/16x16/apps
  icon16.files = images/16x16/$${TARGET}.png

  data.path = $$DATADIR/composer
}
else:win32 {
  INSTALLBASE = "C:/Composer"

  data.path = $$INSTALLBASE/data
}

data.files = data/defaultConnBase.ncl data/style.qss

DEFINES += EXT_DEFAULT_PATH=\"\\\"$$PREFIX\\\"\"
DEFINES += INSTALL_PATH=\"\\\"$$INSTALLBASE\\\"\"
DEFINES += DATA_PATH=\"\\\"$$data.path\\\"\"

unix:!macx {
    target.path = $$INSTALLBASE/bin

    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer\'
    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer/extensions\'
}
else {
    target.path = $$INSTALLBASE
}

INCLUDEPATH +=  include

INCLUDEPATH   +=  ../composer-core/core/include \
                  ../composer-core
LIBS          +=  -L../composer-core/core

macx {
    LIBS += -framework ComposerCore
   
    INCLUDEPATH += \
        /Library/Frameworks/ComposerCore.framework/ \
        /opt/local/include/

    runssh_on {
      message ("NCL Composer.pro will be built with support to run over SSH.")
      DEFINES += WITH_LIBSSH2
      LIBS += -L/opt/local/lib -lssh2 -lgcrypt
    }
}
else:unix {
    LIBS += -L$$INSTALLBASE/lib/composer -lComposerCore
    INCLUDEPATH +=  $$INSTALLBASE/include/composer \
                    $$INSTALLBASE/include/composer/core

    runssh_on {
      message ("NCL Composer.pro will be built with support to run over SSH.")
      DEFINES += WITH_LIBSSH2
      LIBS += -lssh2
    }
}
else:win32 {
    LIBS += -L$$INSTALLBASE -lComposerCore1
    INCLUDEPATH += $$INSTALLBASE/include/composer \
                   $$INSTALLBASE/include/composer/core

    runssh_on {
      message ("NCL Composer.pro will be built with support to run over SSH.")
      # Link against libssh2
      DEFINES += WITH_LIBSSH2
      LIBS += deps/libssh2-1.3.0/lib/libssh2.a \
              deps/libssh2-1.3.0/lib/libgcrypt.a \
              deps/libssh2-1.3.0/lib/libgpg-error.a \
              -lws2_32
      INCLUDEPATH += deps/libssh2-1.3.0/include
    }
}

clubencl {
    DEFINES += WITH_CLUBENCL
    #if clube ncl
    LIBS += -lquazip
}

runssh_on {
  DEFINES += WITH_LIBSSH2

  SOURCES +=    src/SimpleSSHClient.cpp \
                src/RunRemoteGingaVM.cpp
  HEADERS +=    include/SimpleSSHClient.h \
                include/RunRemoteGingaVM.h
}

SOURCES += main.cpp \
    src/ComposerMainWindow.cpp \
    src/PreferencesDialog.cpp \
    src/PerspectiveManager.cpp \
    src/PluginDetailsDialog.cpp \
    src/EnvironmentPreferencesWidget.cpp \
    src/WelcomeWidget.cpp \
    src/AboutDialog.cpp \
    src/RunGingaConfig.cpp \
    src/ComposerHelpWidget.cpp \
    src/GeneralPreferences.cpp \
    src/NewProjectWizard.cpp \
    src/FileChooser.cpp \
    src/ClickableDockWidget.cpp \
    src/ComposerSplashScreen.cpp
#   src/ImportBasePreferences.cpp

HEADERS += include/ComposerMainWindow.h \
    include/PreferencesDialog.h \
    include/PerspectiveManager.h \
    include/PluginDetailsDialog.h \
    include/EnvironmentPreferencesWidget.h \
    include/IPreferencesPage.h \
    include/WelcomeWidget.h \
    include/AboutDialog.h \
    include/RunGingaConfig.h \
    include/ComposerHelpWidget.h \
    include/GeneralPreferences.h \
    include/NewProjectWizard.h \
    include/FileChooser.h \
    include/ClickableDockWidget.h \
    include/ComposerSplashScreen.h
#   include/ImportBasePreferences.h

RESOURCES += images.qrc

FORMS   += ui/PreferencesDialog.ui \
    ui/ComposerMainWindow.ui \
    ui/PerspectiveManager.ui \
    ui/RunGingaConfig.ui \
    ui/PluginDetailsDialog.ui \
    ui/EnvironmentPreferencesWidget.ui \
    ui/WelcomeWidget.ui \
    ui/AboutDialog.ui \
    ui/GeneralPreferences.ui \
    ui/NewProjectWizard.ui \
    ui/FileChooser.ui

#TRANSLATIONS
win32 {
    trans.path = $$INSTALLBASE/extensions

} else:macx{
    trans.path = "/Library/Application Support/Composer/Extensions"

}else:unix {
    trans.path = $$INSTALLBASE/lib/composer/extensions
}

trans.files = translations/*.qm

isEmpty(trans.path) {
    trans.path = $(QTDIR)/translations
}

TRANSLATIONS += translations/composer_pt_BR.ts \
                translations/composer_es_ES.ts

INSTALLS += target trans data

unix:!macx {
    INSTALLS += target desktop
    INSTALLS += icon256 icon128 icon64 icon48 icon32 icon24 icon22 icon16
}

OTHER_FILES += LICENSE.LGPL
