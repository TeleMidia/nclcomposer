CONFIG += link_ncl_profile

include(../plugins-common.pri)

QT += \
  widgets \
  xml

INCLUDEPATH += \
  src/ \
  src/core/ \
  src/ui/ \
  src/ui/view/ \
  deps/

TEMPLATE = \
    lib

DEFINES += \
   LINK_WITH_PARENT=\"1\"

HEADERS += \
    src/core/Structural.h \
    src/core/StructuralEntity.h \
    src/StructuralViewPlugin.h \
    src/StructuralViewFactory.h \
    src/ui/Insert.h \
    src/ui/view/StructuralGraphicsEntity.h \
    src/ui/Change.h \
    src/ui/view/StructuralConnector.h \
    src/ui/view/StructuralGraphicsActionDialog.h \
    src/ui/view/StructuralGraphicsArea.h \
    src/ui/view/StructuralGraphicsBind.h \
    src/ui/view/StructuralGraphicsBindDialog.h \
    src/ui/view/StructuralGraphicsBody.h \
    src/ui/view/StructuralGraphicsComposition.h \
    src/ui/view/StructuralGraphicsConditionDialog.h \
    src/ui/view/StructuralGraphicsContext.h \
    src/ui/view/StructuralGraphicsEdge.h \
    src/ui/view/StructuralGraphicsInterface.h \
    src/ui/view/StructuralGraphicsLink.h \
    src/ui/view/StructuralGraphicsLinkDialog.h \
    src/ui/view/StructuralGraphicsMapping.h \
    src/ui/view/StructuralGraphicsMedia.h \
    src/ui/view/StructuralGraphicsNode.h \
    src/ui/view/StructuralGraphicsPort.h \
    src/ui/view/StructuralGraphicsProperty.h \
    src/ui/view/StructuralGraphicsReference.h \
    src/ui/view/StructuralGraphicsSwitch.h \
    src/ui/view/StructuralGraphicsSwitchPort.h \
    src/ui/view/StructuralMenu.h \
    src/ui/view/StructuralMiniMap.h \
    src/ui/Remove.h \
    src/ui/view/StructuralScene.h \
    src/ui/view/StructuralUtil.h \
    src/ui/view/StructuralView.h \
    src/ui/view/StructuralViewLink.h

SOURCES += \
    src/core/StructuralEntity.cpp \
    src/StructuralViewFactory.cpp \
    src/StructuralViewPlugin.cpp \
    src/ui/view/Insert.cpp \
    src/ui/view/Change.cpp \
    src/ui/view/StructuralConnector.cpp \
    src/ui/view/StructuralGraphicsActionDialog.cpp \
    src/ui/view/StructuralGraphicsArea.cpp \
    src/ui/view/StructuralGraphicsBind.cpp \
    src/ui/view/StructuralGraphicsBindDialog.cpp \
    src/ui/view/StructuralGraphicsBody.cpp \
    src/ui/view/StructuralGraphicsComposition.cpp \
    src/ui/view/StructuralGraphicsConditionDialog.cpp \
    src/ui/view/StructuralGraphicsContext.cpp \
    src/ui/view/StructuralGraphicsEdge.cpp \
    src/ui/view/StructuralGraphicsEntity.cpp \
    src/ui/view/StructuralGraphicsInterface.cpp \
    src/ui/view/StructuralGraphicsLink.cpp \
    src/ui/view/StructuralGraphicsLinkDialog.cpp \
    src/ui/view/StructuralGraphicsMapping.cpp \
    src/ui/view/StructuralGraphicsMedia.cpp \
    src/ui/view/StructuralGraphicsNode.cpp \
    src/ui/view/StructuralGraphicsPort.cpp \
    src/ui/view/StructuralGraphicsProperty.cpp \
    src/ui/view/StructuralGraphicsReference.cpp \
    src/ui/view/StructuralGraphicsSwitch.cpp \
    src/ui/view/StructuralGraphicsSwitchPort.cpp \
    src/ui/view/StructuralMenu.cpp \
    src/ui/view/StructuralMiniMap.cpp \
    src/ui/view/Remove.cpp \
    src/ui/view/StructuralScene.cpp \
    src/ui/view/StructuralUtil.cpp \
    src/ui/view/StructuralViewLink.cpp \
    src/ui/view/StructuralView.cpp

HEADERS += \
    deps/qncg.h \
    deps/qncggraphicsentity.h \
    deps/qncggraphicsnode.h \
    deps/qncggraphicsinterface.h

SOURCES += \
    deps/qncggraphicsentity.cpp \
    deps/qncggraphicsnode.cpp \
    deps/qncggraphicsinterface.cpp

RESOURCES += \
    ../common-resources/images.qrc

FORMS += \
    ui/QnstGraphicsLinkForm.ui \
    ui/QnstGraphicsConditionForm.ui \
    ui/QnstGraphicsActionForm.ui \
    ui/QnstGraphicsBindForm.ui

OTHER_FILES += \
    ncl-structural-view.json
