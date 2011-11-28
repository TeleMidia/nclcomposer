include(../plugins-common.pri)

QNSTROOT += \
    $$PWD

QNSTINCLUDES += \
    $$QNSTROOT/src/core \
    $$QNSTROOT/../deps/libqncg/src

QNSTLIBS += \
    -L$$QNSTROOT/../deps/libqncg/src -lQncg
