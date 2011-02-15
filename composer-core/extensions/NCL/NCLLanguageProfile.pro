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
CONFIG += debug plugin

macx:INSTALLBASE = /Library/Frameworks
else:unix:INSTALLBASE = /usr/local
win32:INSTALLBASE = C:/Composer

HEADERS_PATH = $$INSTALLBASE/include/composer

INCLUDEPATH += ../../core/include

headers_nclprofile.files += NCLStructure.h

macx {
    LIBS += -L../../core -F../../core \
        -framework ComposerCore
    target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L../../core -lComposerCore
    target.path = $$quote(/usr/local/lib/composer/extension)
    headers_nclprofile.path = $$HEADERS_PATH/core/extensions
}
else:win32 {
    LIBS += -L../../core/debug -lComposerCore1
    HEADERS_PATH_UNIX = $$INSTALLBASE/include/composer
    INCLUDEPATH += C:/Composer/include/composer
    target.path = $$quote(C:/Composer/lib/composer)
}

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += NCLLanguageProfile.cpp \
    NCLDocumentParser.cpp \
    NCLStructure.cpp

HEADERS += NCLLanguageProfile.h\
        NCLStructure.h \
        NCLLanguageProfile_global.h \
        NCLDocumentParser.h

INSTALLS += target headers_nclprofile
