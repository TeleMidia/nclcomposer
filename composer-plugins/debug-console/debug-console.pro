#-------------------------------------------------
#
# Project created by QtCreator 2011-01-23T14:17:36
#
#-------------------------------------------------

TARGET = debug_console

include (../plugins-common.pri)

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += include

SOURCES += src/DebugConsoleFactory.cpp \
    src/DebugConsolePlugin.cpp

HEADERS += include/DebugConsoleFactory.h\
        include/DebugConsole_global.h \
    include/DebugConsolePlugin.h

RESOURCES += images.qrc

INSTALLS += target
