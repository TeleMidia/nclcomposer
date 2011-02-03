#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:59:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = NCLLanguageProfile
TEMPLATE = lib
CONFIG += debug plugin

INCLUDEPATH += ../../core/include

macx {
    LIBS += -L../../core -F../../core \
        -framework ComposerCore
    target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L../../core -lComposerCore
    target.path = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -L../../core/debug -lComposerCore1
    INCLUDEPATH += C:/Composer/include/composer
    target.path = $$quote(C:/Composer/lib/composer)
}

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += NCLLanguageProfile.cpp \
    NCLDocumentParser.cpp

HEADERS += NCLLanguageProfile.h\
        NCLLanguageProfile_global.h \
    NCLDocumentParser.h

INSTALLS += target
