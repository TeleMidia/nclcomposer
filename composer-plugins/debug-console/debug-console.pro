#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T14:17:36
#
#-------------------------------------------------


CONFIG += plugin
TARGET = DebugConsole
TEMPLATE = lib

macx {
    LIBS += -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
    DESTDIR = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += /usr/local/include/composer
    DESTDIR = $$quote(/usr/local/lib/composer/extension)
}

DEFINES += DEBUGCONSOLE_LIBRARY

SOURCES += src/DebugConsoleFactory.cpp \
    src/DebugConsolePlugin.cpp

HEADERS += include/DebugConsoleFactory.h\
        include/DebugConsole_global.h \
    include/DebugConsolePlugin.h

RESOURCES += \
    images.qrc
