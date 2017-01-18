include (../plugins-common.pri)

DEFINES += DEBUGCONSOLE_LIBRARY

INCLUDEPATH += .

SOURCES += \
  DebugConsoleFactory.cpp \
  DebugConsolePlugin.cpp

HEADERS += \
  DebugConsoleFactory.h\
  DebugConsole_global.h \
  DebugConsolePlugin.h

RESOURCES += \
  images.qrc

OTHER_FILES += \
  debug_console.json
