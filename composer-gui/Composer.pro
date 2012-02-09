TARGET = composer
TEMPLATE = app

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
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

#CONFIG += clubencl
CONFIG += runssh_on
QT += core xml network webkit

DEFINES += NCLCOMPOSER_GUI_VERSION=\"\\\"0.1.0\\\"\"
DEFINES += BUILD_DATE=\"\\\"$${_DATE_}\"\\\"
DEFINES += WITH_TEST_VERSION_MESSAGE=\"\\\"1\\\"\"

#DEFINES += USE_MDI
RC_FILE = images/nclcomposer.rc

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx {
  TARGET = Composer
  INSTALLBASE = /Applications
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
else:win32 {
  INSTALLBASE = "C:/Composer"
}

DEFINES += EXT_DEFAULT_PATH=\"\\\"$$PREFIX\\\"\"

unix:!macx {
    target.path = $$INSTALLBASE/bin

    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer\'
    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer/extensions\'
}
else {
    target.path = $$INSTALLBASE
}

INCLUDEPATH +=  include

INCLUDEPATH   +=  ../composer-core/core/include
LIBS          +=  -L../composer-core/core

macx {
    LIBS += -framework ComposerCore
   
   INCLUDEPATH += \
        /Library/Frameworks/ComposerCore.framework/ \
        /opt/local/include/

    runssh_on {
      DEFINES += WITH_LIBSSH2
      LIBS += -L/opt/local/lib -lssh2 -lgcrypt
    }
}
else:unix {
    LIBS += -L$$INSTALLBASE/lib/composer -lComposerCore
    INCLUDEPATH +=  $$INSTALLBASE/include/composer \
                    $$INSTALLBASE/include/composer/core

    runssh_on {
      DEFINES += WITH_LIBSSH2
      LIBS += -lssh2
    }
}
else:win32 {
    LIBS += -L$$INSTALLBASE -lComposerCore1
    INCLUDEPATH += $$INSTALLBASE/include/composer \
                   $$INSTALLBASE/include/composer/core

    runssh_on {
        # Link against libssh2
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
    src/ComposerSettings.cpp

HEADERS += include/ComposerMainWindow.h \
    include/PreferencesDialog.h \
    include/PerspectiveManager.h \
    include/PluginDetailsDialog.h \
    include/EnvironmentPreferencesWidget.h \
    include/IPreferencePage.h \
    include/WelcomeWidget.h \
    include/AboutDialog.h \
    include/RunGingaConfig.h \
    include/ComposerSettings.h

RESOURCES += images.qrc

FORMS   += ui/PreferencesDialog.ui \
    ui/ComposerMainWindow.ui \
    ui/PerspectiveManager.ui \
    ui/RunGingaConfig.ui \
    ui/PluginDetailsDialog.ui \
    ui/EnvironmentPreferencesWidget.ui \
    ui/WelcomeWidget.ui \
    ui/AboutDialog.ui

unix:!macx {
    INSTALLS += target desktop icon64 icon48
} else {
    INSTALLS += target
}

TRANSLATIONS += composer_br.ts \
                composer_sp.ts

OTHER_FILES += LICENSE.LGPL
