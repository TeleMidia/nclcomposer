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
    ui/view/qnstcomposite.h \
    ui/view/qnstnode.h \
    ui/view/qnstbody.h \
    ui/view/qnstswitch.h \
    ui/view/qnstcontext.h \
    ui/view/qnstinterface.h \
    ui/view/qnstport.h \
    ui/view/qnstproperty.h \
    ui/view/qnstarea.h \
    ui/view/qnstmedia.h \
    ui/view/qnstcontent.h \
    ui/view/qnstedge.h \
    ui/view/qnstportreference.h \
    ui/view/qnstentity.h \
    core/qnst.h

SOURCES += \
    qnstcomposerplugin.cpp \
    qnstcomposerpluginfactory.cpp \
    ui/view/qnstview.cpp \
    ui/view/qnstscene.cpp \
    ui/view/qnstcomposite.cpp \
    ui/view/qnstnode.cpp \
    ui/view/qnstbody.cpp \
    ui/view/qnstswitch.cpp \
    ui/view/qnstcontext.cpp \
    ui/view/qnstinterface.cpp \
    ui/view/qnstport.cpp \
    ui/view/qnstproperty.cpp \
    ui/view/qnstarea.cpp \
    ui/view/qnstmedia.cpp \
    ui/view/qnstcontent.cpp \
    ui/view/qnstedge.cpp \
    ui/view/qnstportreference.cpp \
    ui/view/qnstentity.cpp

RESOURCES += \
    resources.qrc

FORMS += \
    forms/qnstdbind.ui
