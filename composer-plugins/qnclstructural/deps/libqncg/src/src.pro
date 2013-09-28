include("../configure.pri")

TEMPLATE = \
    lib

TARGET = \
    Qncg

VERSION = \
    0.1.0

CONFIG += static

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
}

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("libqncg.pro RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("libqncg.pro DEBUG build!")
}

HEADERS += \
    qncg.h \
    qncggraphicsentity.h \
    qncggraphicsnode.h \
    qncggraphicsinterface.h

SOURCES += \
    qncggraphicsentity.cpp \
    qncggraphicsnode.cpp \
    qncggraphicsinterface.cpp

RESOURCES +=








INSTALLS += target
