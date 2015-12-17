TEMPLATE    =   lib
CONFIG      +=  plugin dll silent
CONFIG      -=  debug
MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

GIT_VERSION=true

isEmpty(CPRVERSION) {
  #GUI_VERSION=$$system(git describe --tag | sed "s/v\(.*\)-.*-.*/\1/")
  PLUGINS_VERSION=$$system(git describe --tag | sed "s/v//")
}
else {
  PLUGINS_VERSION=$$CPRVERSION
  GIT_VERSION=false
}

message("NCL Composer Plugins build version $${PLUGINS_VERSION} (from git=$${GIT_VERSION})")

VERSTR = '\\"$${PLUGINS_VERSION}\\"'
DEFINES += NCLCOMPOSER_PLUGINS_VERSION=\"$${VERSTR}\"

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("plugins-common.pri RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("plugins-common.pri DEBUG build!")
}

macx {
  INSTALLBASE = /Applications/Composer
} 
else:unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  INSTALLBASE = $$PREFIX
}
else:win32 {
  INSTALLBASE = "C:/Composer"
}

INCLUDEPATH += . \
               $$PWD/../core/src \
               $$PWD/ncl-profile/ \

LIBS += -L$$PWD/../../bin \
        -L$$PWD/../../bin/extensions

 
macx {
  LIBS += -F/Library/Frameworks \
          -F$$PWD/../../bin -framework ComposerCore

  LIBS += -L$$PWD/../../bin \
          -L$$PWD/../../bin/extensions \
          $$quote(-L/Library/Application Support/Composer/Extensions)

  INCLUDEPATH +=  /Library/Frameworks/ComposerCore.framework/ \
                  /Library/Frameworks/ComposerCore.framework/core \
                  /Library/Frameworks/ComposerCore.framework/core/extensions

  target.path = $$quote(/Library/Application Support/Composer/Extensions)
}
else:unix {
  LIBS += -L$$INSTALLBASE/lib/composer -lComposerCore

  link_ncl_profile {
    LIBS += -L$$INSTALLBASE/lib/composer/extensions -lNCLLanguageProfile
  }

  INCLUDEPATH += $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                 $$INSTALLBASE/include/composer/extensions

  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN\':\'\$\$ORIGIN/../\'
  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer\'
  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer/extensions\'

  target.path = $$quote($$INSTALLBASE/lib/composer/extensions)
}
else:win32 {
  LIBS += -L$$INSTALLBASE/lib/composer -lComposerCore1


  link_ncl_profile {
         LIBS += -L$$INSTALLBASE/extensions -lNCLLanguageProfile
  }

  INCLUDEPATH += $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                 $$INSTALLBASE/include/composer/extensions

  target.path = $$INSTALLBASE/extensions
}

# Set the name of the default target based on the .pro file
DEFAULT_TARGET=$$basename(_PRO_FILE_)
DEFAULT_TARGET=$$replace(DEFAULT_TARGET, "-", "_")
DEFAULT_TARGET=$$replace(DEFAULT_TARGET, ".pro", "")
contains(FORCERELEASE, true) {
  TARGET=$$DEFAULT_TARGET
}
else {
  TARGET=$${DEFAULT_TARGET}_d
}

INSTALLS += target
DESTDIR  = $$PWD/../../bin/extensions/
