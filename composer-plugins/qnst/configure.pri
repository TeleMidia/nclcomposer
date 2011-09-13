include(../plugins-common.pri)

QNSTROOT += \
    $$PWD

QNSTINCLUDES += \
    $$QNSTROOT/../deps/libqncg/src

QNSTLIBS += \
    -L$$QNSTROOT/../deps/libqncg/src -lQncg
