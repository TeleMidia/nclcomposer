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
    src/NCLLayoutViewPlugin.cpp \
    src/NCLLayoutViewFactory.cpp \
    src/ui/MainWindow.cpp \
    src/ui/view/Canvas.cpp \
    src/ui/view/Grid.cpp \
    src/ui/view/View.cpp \
    src/ui/view/Region.cpp \
    src/ui/view/RegionBase.cpp

HEADERS += \
    src/NCLLayoutViewPlugin.h \
    src/NCLLayoutViewFactory.h \
    src/ui/MainWindow.h \
    src/ui/view/Canvas.h \
    src/ui/view/View.h \
    src/ui/view/Grid.h \
    src/ui/view/RegionBase.h \
    src/ui/view/Region.h

INSTALLS = \
  target

FORMS += \
  ui/MainWindow.ui

OTHER_FILES += \
  ncl-layout-view.json
