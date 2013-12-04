include(../../plugins-common.pri)

QT += \
    svg \
    xml

TEMPLATE = \
    lib

TARGET = \
    nclstructural

DEFINES += \
    PLUGIN

INCLUDEPATH += \
    src/ \
    src/ui/ \
    src/ui/view \
    src/core/ \

HEADERS += \
    src/ui/qnstmainwindow.h \
    src/core/qnstinsert.h \
    src/core/qnstremove.h \
    src/core/qnstchange.h \
    src/core/qnstcommand.h \
    src/ui/view/qnstview.h \
    src/ui/view/qnstscene.h \
    src/core/qnst.h \
    src/ui/view/qnstnode.h \
    src/ui/view/qnstcomposition.h \
    src/ui/view/qnstmedia.h \
    src/ui/view/qnstentity.h \
    src/ui/view/qnstmenu.h \
    src/ui/view/qnstinterface.h \
    src/ui/view/qnstviewlink.h \
    src/ui/view/qnstreference.h \
    src/ui/view/qnstbind.h \
    src/ui/view/qnstedge.h \
    src/ui/view/qnstlink.h \
    src/ui/view/qnstconditiondialog.h \
    src/ui/view/qnstutil.h \
    src/ui/view/qnstminimap.h

HEADERS += \
    src/qnstcomposerplugin.h \
    src/qnstcomposerpluginfactory.h \

SOURCES += \
    src/ui/qnstmainwindow.cpp \
    src/core/qnstinsert.cpp \
    src/core/qnstremove.cpp \
    src/core/qnstchange.cpp \
    src/core/qnstcommand.cpp \
    src/ui/view/qnstview.cpp \
    src/ui/view/qnstscene.cpp \
    src/ui/view/qnstnode.cpp \
    src/ui/view/qnstcomposition.cpp \
    src/ui/view/qnstmedia.cpp \
    src/ui/view/qnstentity.cpp \
    src/ui/view/qnstmenu.cpp \
    src/ui/view/qnstinterface.cpp \
    src/ui/view/qnstviewlink.cpp \
    src/ui/view/qnstreference.cpp \
    src/ui/view/qnstbind.cpp \
    src/ui/view/qnstedge.cpp \
    src/ui/view/qnstlink.cpp \
    src/ui/view/qnstutil.cpp \
    src/ui/view/qnstminimap.cpp

SOURCES += \
    src/qnstcomposerplugin.cpp \
    src/qnstcomposerpluginfactory.cpp \

RESOURCES += \
    application.qrc

RESOURCES += \
    ../../common-resources/images.qrc
