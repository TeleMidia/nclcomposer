include (../plugins-common.pri)

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += \
  src

SOURCES += \
  src/DebugConsoleFactory.cpp \
  src/DebugConsolePlugin.cpp

HEADERS += \
  src/DebugConsoleFactory.h\
  src/DebugConsole_global.h \
  src/DebugConsolePlugin.h

RESOURCES += \
  images.qrc

OTHER_FILES += \
    debug_console.json
