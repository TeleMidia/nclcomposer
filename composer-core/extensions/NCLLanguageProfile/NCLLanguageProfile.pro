#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:59:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = NCLLanguageProfile
TEMPLATE = lib
CONFIG += plugin


macx {
    LIBS += -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
    DESTDIR = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += $$LOCATION/include/composer
    DESTDIR = $$quote(/usr/local/lib/composer/extension)
}

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += NCLLanguageProfile.cpp \
    NCLDocumentParser.cpp

HEADERS += NCLLanguageProfile.h\
        NCLLanguageProfile_global.h \
    NCLDocumentParser.h
