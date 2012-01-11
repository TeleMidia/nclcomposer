include("../configure.pri")

QT += \
    svg \
    xml

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
    ui/view/qnstgraphicsentity.h \
    ui/view/qnstmenu.h \
    ui/view/qnstgraphicsinterface.h \
    ui/view/qnstgraphicsport.h \
    ui/view/qnstviewlink.h \
    ui/view/qnstgraphicsreference.h \
    ui/view/qnstgraphicsbind.h \
    ui/view/qnstgraphicsedge.h \
    ui/view/qnstgraphicslink.h \
    ui/view/qnstgraphicslinkdialog.h \
    ui/view/qnstconncetor.h \
    ui/view/qnstgraphicsaggregator.h \
    ui/view/qnstgraphicsconditiondialog.h \
    ui/view/qnstgraphicsactiondialog.h \
    ui/view/qnstgraphicscondition.h \
    ui/view/qnstgraphicsaction.h \
    ui/view/qnstlink.h \
    ui/view/qnstbind.h

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
    ui/view/qnstgraphicsentity.cpp \
    ui/view/qnstmenu.cpp \
    ui/view/qnstgraphicsinterface.cpp \
    ui/view/qnstgraphicsport.cpp \
    ui/view/qnstviewlink.cpp \
    ui/view/qnstgraphicsreference.cpp \
    ui/view/qnstgraphicsbind.cpp \
    ui/view/qnstgraphicsedge.cpp \
    ui/view/qnstgraphicslink.cpp \
    ui/view/qnstgraphicslinkdialog.cpp \
    ui/view/qnstconncetor.cpp \
    ui/view/qnstgraphicsaggregator.cpp \
    ui/view/qnstgraphicsconditiondialog.cpp \
    ui/view/qnstgraphicsactiondialog.cpp \
    ui/view/qnstgraphicscondition.cpp \
    ui/view/qnstgraphicsaction.cpp \
    ui/view/qnstlink.cpp \
    ui/view/qnstbind.cpp

RESOURCES += \
    ../../common-resources/images.qrc

FORMS += \
    ui/view/QnstGraphicsLinkForm.ui \
    ui/view/QnstGraphicsConditionForm.ui \
    ui/view/QnstGraphicsActionForm.ui







































