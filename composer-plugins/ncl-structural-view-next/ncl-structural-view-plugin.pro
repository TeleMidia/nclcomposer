include(../plugins-common.pri)

QT += \
  widgets \
  xml

TARGET = \
  ncl-structural-view-next

DESTDIR = \
  bin/

DEFINES += \
  PLUGIN

INCLUDEPATH += \
  src/ \
  src/core/ \
  src/ui/ \
  src/ui/view/ \

HEADERS += \
  src/core/Command.h \
    src/core/Structural.h \
    src/core/Remove.h \
    src/core/Insert.h \
    src/core/Change.h \
    src/StructuralViewPlugin.h \
    src/StructuralViewFactory.h \
    src/ui/StructuralWindow.h \
    src/ui/view/StructuralViewLink.h \
    src/ui/view/StructuralView.h \
    src/ui/view/StructuralUtil.h \
    src/ui/view/StructuralScene.h \
    src/ui/view/StructuralReference.h \
    src/ui/view/StructuralNode.h \
    src/ui/view/StructuralMinimap.h \
    src/ui/view/StructuralMenu.h \
    src/ui/view/StructuralMedia.h \
    src/ui/view/StructuralLink.h \
    src/ui/view/StructuralInterface.h \
    src/ui/view/StructuralEntity.h \
    src/ui/view/StructuralEdge.h \
    src/ui/view/StructuralConditionDialog.h \
    src/ui/view/StructuralComposition.h \
    src/ui/view/StructuralBind.h \
    src/ui/view/StructuralBindDialog.h

SOURCES += \
    src/core/Command.cpp \
    src/StructuralViewPlugin.cpp \
    src/StructuralViewFactory.cpp \
    src/ui/StructuralWindow.cpp \
    src/ui/view/StructuralViewLink.cpp \
    src/ui/view/StructuralView.cpp \
    src/ui/view/StructuralUtil.cpp \
    src/ui/view/StructuralScene.cpp \
    src/ui/view/StructuralReference.cpp \
    src/ui/view/StructuralNode.cpp \
    src/ui/view/StructuralMinimap.cpp \
    src/ui/view/StructuralMenu.cpp \
    src/ui/view/StructuralMedia.cpp \
    src/ui/view/StructuralLink.cpp \
    src/ui/view/StructuralInterface.cpp \
    src/ui/view/StructuralEntity.cpp \
    src/ui/view/StructuralEdge.cpp \
    src/ui/view/StructuralComposition.cpp \
    src/ui/view/StructuralBind.cpp \
    src/core/Remove.cpp \
    src/core/Insert.cpp \
    src/core/Change.cpp \
    src/ui/view/StructuralBindDialog.cpp

RESOURCES += \
  ncl-structural-view-next.qrc \
  ../common-resources/images.qrc

FORMS += \
    ui/StructuralBindDialog.ui

INSTALLS = \
  target
