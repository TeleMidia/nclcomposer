include(../plugins-common.pri)

QT += \
  widgets \
  xml

TARGET = \
  ncl-structural-view

DESTDIR = \
  bin/

INCLUDEPATH += \
  src/ \
  src/core/ \
  src/ui/ \
  src/ui/view/ \

HEADERS += \
  src/ui/MainWindow.h \
  src/core/Command.h \
  src/core/Globals.h \
  src/core/ChangeCommand.h \
  src/core/InsertCommand.h \
  src/core/RemoveCommand.h \
  src/ui/view/View.h \
  src/ui/view/Bind.h \
  src/ui/view/Composition.h \
  src/ui/view/ConditionDialog.h \
  src/ui/view/Edge.h \
  src/ui/view/Entity.h \
  src/ui/view/Interface.h \
  src/ui/view/Link.h \
  src/ui/view/Media.h \
  src/ui/view/Menu.h \
  src/ui/view/Node.h \
  src/ui/view/Reference.h \
  src/ui/view/ViewLink.h \
  src/ui/view/Util.h \
  src/ui/view/Scene.h \
  src/ui/view/Minimap.h \
  src/NCLStructuralViewPlugin.h \
  src/NCLStructuralViewFactory.h

SOURCES += \
  src/ui/MainWindow.cpp \
  src/core/Command.cpp \
  src/core/ChangeCommand.cpp \
  src/core/InsertCommand.cpp \
  src/core/RemoveCommand.cpp \
  src/ui/view/Bind.cpp \
  src/ui/view/Composition.cpp \
  src/ui/view/Edge.cpp \
  src/ui/view/Entity.cpp \
  src/ui/view/Interface.cpp \
  src/ui/view/Link.cpp \
  src/ui/view/Media.cpp \
  src/ui/view/Menu.cpp \
  src/ui/view/Minimap.cpp \
  src/ui/view/Node.cpp \
  src/ui/view/Reference.cpp \
  src/ui/view/Scene.cpp \
  src/ui/view/Util.cpp \
  src/ui/view/View.cpp \
  src/ui/view/ViewLink.cpp \
  src/NCLStructuralViewPlugin.cpp \
  src/NCLStructuralViewFactory.cpp

RESOURCES += \
  ncl-structural-view.qrc \
  ../common-resources/images.qrc

INSTALLS = \
  target
