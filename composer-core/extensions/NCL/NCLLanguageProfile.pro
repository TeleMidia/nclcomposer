#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:59:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

MOC_DIR = .mocs
OBJECTS_DIR = .objs

TARGET = NCLLanguageProfile
TEMPLATE = lib
CONFIG += plugin dll

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("NCLLanguageProfile.pro RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("NCLLanguageProfile.pro DEBUG build!")
}

macx {
  INSTALLBASE = /Library/Frameworks
  INSTALLSUPPORT = $$quote(/Library/Application Support/Composer)
}
else:unix {
 isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
 
  INSTALLBASE = $$PREFIX
}
else:win32 {
  INSTALLBASE = C:/Composer
}

macx:HEADERS_PATH = $$INSTALLBASE/ComposerCore.framework
else:unix:HEADERS_PATH = $$INSTALLBASE/include/composer

INCLUDEPATH += ../../core/include

headers_nclprofile.files += NCLLanguageProfile.h\
                            NCLStructure.h \
                            NCLLanguageProfile_global.h \
                            NCLDocumentParser.h

macx {
    LIBS += -L../../core -F../../core -framework ComposerCore
    target.path = $$INSTALLSUPPORT
    headers_nclprofile.path = $$HEADERS_PATH/core/extensions

    QMAKE_LFLAGS += -Wl,-install_name,'\'$$INSTALLSUPPORT/lib'$$TARGET'.dylib\''
}
else:unix {
    QMAKE_LFLAGS += -Wl,-rpath,\'\$\$ORIGIN/../lib/composer\'
    LIBS += -L../../core -lComposerCore
    target.path = $$quote($$INSTALLBASE/lib/composer/extensions)
    headers_nclprofile.path = $$HEADERS_PATH/core/extensions
}
else:win32 {
    LIBS += -L$$INSTALLBASE -lComposerCore1
    HEADERS_PATH = $$INSTALLBASE/include/composer
    INCLUDEPATH += C:/Composer/include/composer
    headers_nclprofile.path = $$HEADERS_PATH/core/extensions
    target.path = $$quote(C:/Composer/lib/composer)
}

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += NCLLanguageProfile.cpp \
           NCLDocumentParser.cpp \
           NCLStructure.cpp


HEADERS +=  NCLLanguageProfile.h \
            NCLLanguageProfile_global.h \
            NCLDocumentParser.h \
            NCLStructure.h


INSTALLS += target headers_nclprofile
