include("../configure.pri")

TEMPLATE = \
    lib

TARGET = \
    Qncg

VERSION = \
    0.1.0

CONFIG += \
    debug \
    static

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
