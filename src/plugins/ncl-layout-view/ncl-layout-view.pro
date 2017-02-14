CONFIG += link_ncl_profile

include (../plugins-common.pri)

INCLUDEPATH += \
  . \
  ui/ \
  ui/view/

SOURCES += \
  LayoutViewPlugin.cpp \
  LayoutViewFactory.cpp \
  ui/view/LayoutView.cpp \
  ui/view/LayoutRegion.cpp \
  ui/view/LayoutRegionBase.cpp \
  ui/view/LayoutGrid.cpp \
  ui/view/LayoutCanvas.cpp \
  ui/LayoutWindow.cpp

HEADERS += \
  LayoutViewPlugin.h \
  LayoutViewFactory.h \
  ui/view/LayoutCanvas.h \
  ui/view/LayoutGrid.h \
  ui/view/LayoutRegion.h \
  ui/view/LayoutRegionBase.h \
  ui/view/LayoutView.h \
  ui/LayoutWindow.h

FORMS += \
  ui/LayoutWindow.ui

OTHER_FILES += \
  ncl-layout-view.json

RESOURCES += \
  ../common-resources/images.qrc

