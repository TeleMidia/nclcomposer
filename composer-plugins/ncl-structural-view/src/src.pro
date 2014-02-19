include("../configure.pri")

QT += \
    svg \
    xml

TEMPLATE = \
    lib

VERSION = \
    0.1.0

DEFINES += \
   LINK_WITH_PARENT=\"1\"

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
    ui/view/qnstgraphicsmedia.h \
    ui/view/qnstgraphicscontext.h \
    ui/view/qnstgraphicsswitch.h \
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
    ui/view/qnstconnector.h \
    ui/view/qnstgraphicsconditiondialog.h \
    ui/view/qnstgraphicsactiondialog.h \
    ui/view/qnstgraphicsarea.h \
    ui/view/qnstgraphicsproperty.h \
    ui/view/qnstaddcommand.h \
    ui/view/qnstchangecommand.h \
    ui/view/qnstremovecommand.h \
    ui/view/qnstgraphicsmapping.h \
    ui/view/qnstgraphicsswitchport.h \
    ui/view/qnstgraphicsbinddialog.h \
    ui/view/qnstutil.h \
    ui/view/qnstminimap.h

SOURCES += \
    qnstcomposerplugin.cpp \
    qnstcomposerpluginfactory.cpp \
    ui/view/qnstview.cpp \
    ui/view/qnstscene.cpp \
    core/qnstentity.cpp \
    ui/view/qnstgraphicsnode.cpp \
    ui/view/qnstgraphicscomposition.cpp \
    ui/view/qnstgraphicsbody.cpp \
    ui/view/qnstgraphicsmedia.cpp \
    ui/view/qnstgraphicscontext.cpp \
    ui/view/qnstgraphicsswitch.cpp \
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
    ui/view/qnstconnector.cpp \
    ui/view/qnstgraphicsconditiondialog.cpp \
    ui/view/qnstgraphicsactiondialog.cpp \
    ui/view/qnstgraphicsarea.cpp \
    ui/view/qnstgraphicsproperty.cpp \
    ui/view/qnstaddcommand.cpp \
    ui/view/qnstchangecommand.cpp \
    ui/view/qnstremovecommand.cpp \
    ui/view/qnstgraphicsmapping.cpp \
    ui/view/qnstgraphicsswitchport.cpp \
    ui/view/qnstgraphicsbinddialog.cpp \
    ui/view/qnstutil.cpp \
    ui/view/qnstminimap.cpp

RESOURCES += \
    ../../common-resources/images.qrc

FORMS += \
    ui/view/QnstGraphicsLinkForm.ui \
    ui/view/QnstGraphicsConditionForm.ui \
    ui/view/QnstGraphicsActionForm.ui \
    ui/view/QnstGraphicsBindForm.ui

OTHER_FILES += \
    ncl_structural_view.json


























































