#-------------------------------------------------
#
# Project created by QtCreator 2011-01-21T13:59:28
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = cpModelProfile
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

DEFINES += CPMODELPROFILE_LIBRARY

SOURCES += \
    cpModelProfile.cpp

HEADERS += \
    cpModelProfile_global.h \
    cpModelProfile.h

INSTALLS += target
