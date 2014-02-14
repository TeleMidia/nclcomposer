include (../plugins-common.pri)

INCLUDEPATH += \
  src/ \
  src/ui/ \
  src/ui/view/

DESTDIR = \
  bin/

TARGET = \
  ncl-layout-view

SOURCES += \
    src/LayoutViewPlugin.cpp \
    src/LayoutViewFactory.cpp \
    src/ui/view/LayoutView.cpp \
    src/ui/view/LayoutRegion.cpp \
    src/ui/view/LayoutRegionBase.cpp \
    src/ui/view/LayoutGrid.cpp \
    src/ui/view/LayoutCanvas.cpp \
    src/ui/LayoutWindow.cpp

HEADERS += \
    src/LayoutViewPlugin.h \
    src/LayoutViewFactory.h \
    src/ui/view/LayoutCanvas.h \
    src/ui/view/LayoutGrid.h \
    src/ui/view/LayoutRegion.h \
    src/ui/view/LayoutRegionBase.h \
    src/ui/view/LayoutView.h \
    src/ui/LayoutWindow.h

INSTALLS = \
  target

FORMS += \
    ui/LayoutWindow.ui

OTHER_FILES += \
  ncl-layout-view.json
