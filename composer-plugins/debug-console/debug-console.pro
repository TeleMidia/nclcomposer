include (../plugins-common.pri)
TARGET = debug_console

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += include

SOURCES += src/DebugConsoleFactory.cpp \
    src/DebugConsolePlugin.cpp

HEADERS += include/DebugConsoleFactory.h\
        include/DebugConsole_global.h \
    include/DebugConsolePlugin.h

RESOURCES += images.qrc

INSTALLS += target
