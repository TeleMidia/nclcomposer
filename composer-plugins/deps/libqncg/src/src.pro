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
    debug \
    staticlib

HEADERS += \
    qncg.h \
    qncgentity.h \
    qncgnode.h \
    qncginterface.h \
    qncgedge.h \
    qncgrectangle.h \
    qncgrectangleinterface.h

SOURCES += \
    qncgentity.cpp \
    qncgnode.cpp \
    qncginterface.cpp \
    qncgedge.cpp \
    qncgrectangle.cpp \
    qncgrectangleinterface.cpp
