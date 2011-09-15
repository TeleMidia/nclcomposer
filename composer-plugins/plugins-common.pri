TEMPLATE    =   lib
CONFIG      +=  qt debug plugin

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

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
  LIBS +=  $$quote(-L/Library/Application Support/Composer)
  INCLUDEPATH +=  include /Library/Frameworks/ComposerCore.framework/ \
                  /Library/Frameworks/ComposerCore.framework/core \
                  /Library/Frameworks/ComposerCore.framework/core/extensions

  target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
  LIBS += -L$$INSTALLBASE/lib/composer \
          -L$$INSTALLBASE/lib/composer/extensions -lNCLLanguageProfile

  INCLUDEPATH += include $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                 $$INSTALLBASE/include/composer/core/extensions

  QMAKE_LFLAGS += -Wl,--rpath=\'\$\$ORIGIN\'
  QMAKE_LFLAGS += --rpath=\'\$\$ORIGIN/../\'
  QMAKE_LFLAGS += --rpath=\'\$\$ORIGIN/../lib/composer\'
  QMAKE_LFLAGS += --rpath=\'\$\$ORIGIN/../lib/composer/extensions\'

  target.path = $$quote($$INSTALLBASE/lib/composer/extensions)
}
else:win32 {
  LIBS += -L$$INSTALLBASE \
          -lComposerCore1 \
          -L$$INSTALLBASE/lib/composer \
          -L$$INSTALLBASE/lib/composer/extensions

  INCLUDEPATH += . include $$INSTALLBASE/include/composer \
                 $$INSTALLBASE/include/composer/core \
                   $$INSTALLBASE/include/composer/core/extensions

  target.path = $$INSTALLBASE/lib/composer
}

INSTALLS = target
