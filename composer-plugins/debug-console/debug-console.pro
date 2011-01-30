#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T14:17:36
#
#-------------------------------------------------

TARGET = debug_console
TEMPLATE = lib
CONFIG += debug plugin

MOC_DIR     = .mocs
OBJECTS_DIR = .objs

macx {
    LIBS += -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/ \
        include/
    target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += /usr/local/include/composer \
         include/
    target.path = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -LC:/composer/lib/composer \
        -lComposerCore1
    INCLUDEPATH += C:/composer/include/composer \
                include/
    target.path = $$quote(C:/composer/lib/composer/)
}

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += include

SOURCES += src/DebugConsoleFactory.cpp \
    src/DebugConsolePlugin.cpp

HEADERS += include/DebugConsoleFactory.h\
        include/DebugConsole_global.h \
    include/DebugConsolePlugin.h

RESOURCES += images.qrc

INSTALLS += target
