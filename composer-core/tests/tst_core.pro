QT -= gui
QT += testlib
CONFIG += testcase

macx {
    LIBS += -L../core \
            -F../core \
            -framework \
            ComposerCore
    INCLUDEPATH += ../core/include
}
else:unix {
    LIBS += -L../core \
        -lComposerCore
    INCLUDEPATH += ../core/include
}

HEADERS += \
    tst_Module.h

SOURCES += \
    tst_Module.cpp
