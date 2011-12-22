include("../configure.pri")

QT += \
    svg

TEMPLATE = \
    lib

TARGET = \
    Qncg

VERSION = \
    0.1.0

CONFIG += \
    release \
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

target.path = .

INSTALLS += target
