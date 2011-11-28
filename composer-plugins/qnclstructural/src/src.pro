include("../configure.pri")

QT += \
    svg

TEMPLATE = \
    lib

TARGET = \
    Qnst

VERSION = \
    0.1.0

INCLUDEPATH += \
    $$QNSTINCLUDES

LIBS += \
    $$QNSTLIBS

HEADERS += \
    qnstcomposerplugin.h \
    qnstcomposerpluginfactory.h \
    ui/view/qnstview.h \
    ui/view/qnstscene.h \
    core/qnst.h \
    core/qnstentity.h \
    ui/view/qnstgraphicsnode.h \
    ui/view/qnstgraphicscomposition.h \
    ui/view/qnstgraphicsbody.h \
    ui/view/qnstgraphicscontent.h \
    ui/view/qnstgraphicsmedia.h \
    ui/view/qnstgraphicsimage.h \
    ui/view/qnstgraphicsvideo.h \
    ui/view/qnstgraphicsaudio.h \
    ui/view/qnstgraphicstext.h \
    ui/view/qnstgraphicsscript.h \
    ui/view/qnstgraphicscontext.h \
    ui/view/qnstgraphicsswitch.h \
    ui/view/qnstgraphicssettings.h \
    ui/view/qnstgraphicsinterface.h \
    ui/view/qnstgraphicsarea.h \
    ui/view/qnstgraphicsport.h \
    ui/view/qnstgraphicsproperty.h

SOURCES += \
    qnstcomposerplugin.cpp \
    qnstcomposerpluginfactory.cpp \
    ui/view/qnstview.cpp \
    ui/view/qnstscene.cpp \
    core/qnstentity.cpp \
    ui/view/qnstgraphicsnode.cpp \
    ui/view/qnstgraphicscomposition.cpp \
    ui/view/qnstgraphicsbody.cpp \
    ui/view/qnstgraphicscontent.cpp \
    ui/view/qnstgraphicsmedia.cpp \
    ui/view/qnstgraphicsimage.cpp \
    ui/view/qnstgraphicsvideo.cpp \
    ui/view/qnstgraphicsaudio.cpp \
    ui/view/qnstgraphicstext.cpp \
    ui/view/qnstgraphicsscript.cpp \
    ui/view/qnstgraphicscontext.cpp \
    ui/view/qnstgraphicsswitch.cpp \
    ui/view/qnstgraphicssettings.cpp \
    ui/view/qnstgraphicsinterface.cpp \
    ui/view/qnstgraphicsport.cpp \
    ui/view/qnstgraphicsproperty.cpp \
    ui/view/qnstgraphicsarea.cpp

RESOURCES += \
    images.qrc
