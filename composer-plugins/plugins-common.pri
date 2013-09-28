TEMPLATE    =   lib
CONFIG      +=  plugin dll
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

macx {
  LIBS += -framework ComposerCore
  LIBS +=  $$quote(-L/Library/Application Support/Composer/Extensions)
  INCLUDEPATH +=  include /Library/Frameworks/ComposerCore.framework/ \
                  /Library/Frameworks/ComposerCore.framework/core \
                  /Library/Frameworks/ComposerCore.framework/core/extensions

  target.path = $$quote(/Library/Application Support/Composer/Extensions)
}
else:unix {
  LIBS += -L$$INSTALLBASE/lib/composer \
          -L$$INSTALLBASE/lib/composer/extensions -lNCLLanguageProfile

  INCLUDEPATH += include $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                 $$INSTALLBASE/include/composer/core/extensions

  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN\':\'\$\$ORIGIN/../\'
  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer\'
  QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer/extensions\'

  target.path = $$quote($$INSTALLBASE/lib/composer/extensions)
}
else:win32 {
  LIBS += -L$$INSTALLBASE -lComposerCore1 \
          -L$$INSTALLBASE/extensions

  INCLUDEPATH += . include $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                 $$INSTALLBASE/include/composer/core/extensions

  target.path = $$INSTALLBASE/extensions
}

INSTALLS = target
