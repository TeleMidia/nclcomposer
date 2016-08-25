include(../plugins-common.pri)

QT += \
    widgets \
    xml

TARGET = \
    ncl-structural-view-next

DESTDIR = \
    bin/

# Uncomment the next two line to enable graphviz as default option
#CONFIG += \
#    graphviz

INCLUDEPATH += \
    src/ \
    src/core/ \
    src/ui/ \
    src/ui/view/ \
    src/util/

HEADERS += \
    src/core/Command.h \
    src/core/Structural.h \
    src/core/Remove.h \
    src/core/Insert.h \
    src/core/Change.h \
    src/StructuralViewPlugin.h \
    src/StructuralViewFactory.h \
    src/ui/StructuralWindow.h \
    src/ui/view/StructuralView.h \
    src/util/StructuralUtil.h \
    src/ui/view/StructuralScene.h \
    src/ui/view/StructuralEdge.h \
    src/ui/view/StructuralNode.h \
    src/ui/view/StructuralMinimap.h \
    src/ui/view/StructuralMenu.h \
    src/ui/view/StructuralInterface.h \
    src/ui/view/StructuralLink.h \
    src/ui/view/StructuralBind.h \
    src/ui/view/StructuralReference.h \
    src/ui/view/StructuralViewLink.h \
    src/ui/view/StructuralEntity.h \
    src/ui/view/StructuralComposition.h \
    src/ui/view/StructuralContent.h \
    src/ui/view/StructuralLinkDialog.h

SOURCES += \
    src/core/Command.cpp \
    src/StructuralViewPlugin.cpp \
    src/StructuralViewFactory.cpp \
    src/ui/StructuralWindow.cpp \
    src/ui/view/StructuralView.cpp \
    src/util/StructuralUtil.cpp \
    src/ui/view/StructuralScene.cpp \
    src/ui/view/StructuralNode.cpp \
    src/ui/view/StructuralMinimap.cpp \
    src/ui/view/StructuralEdge.cpp \
    src/ui/view/StructuralMenu.cpp \
    src/ui/view/StructuralInterface.cpp \
    src/ui/view/StructuralEntity.cpp \
    src/ui/view/StructuralLink.cpp \
    src/ui/view/StructuralBind.cpp \
    src/ui/view/StructuralReference.cpp \
    src/ui/view/StructuralViewLink.cpp \
    src/ui/view/StructuralComposition.cpp \
    src/core/Remove.cpp \
    src/core/Insert.cpp \
    src/core/Change.cpp \
    src/ui/view/StructuralContent.cpp \
    src/ui/view/StructuralLinkDialog.cpp

RESOURCES += \
    ncl-structural-view-next.qrc \
     ../common-resources/images.qrc

FORMS += \
    ui/StructuralLinkForm.ui

INSTALLS = \
    target

graphviz {
  unix {
    DEFINES += \
      WITH_GRAPHVIZ

    INCLUDEPATH += \
      /usr/include/graphviz/ \
      /usr/local/include/graphviz/

    LIBS += \
      -lgvc -lcgraph
  }
}
