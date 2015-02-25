include (../plugins-common.pri)

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += include

SOURCES += src/DebugConsoleFactory.cpp \
    src/DebugConsolePlugin.cpp

HEADERS += include/DebugConsoleFactory.h\
        include/DebugConsole_global.h \
    include/DebugConsolePlugin.h

RESOURCES += images.qrc

OTHER_FILES += \
    debug_console.json
