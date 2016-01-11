CONFIG += link_ncl_profile
include (../plugins-common.pri)

QT              += xml

win32:LIBS += -lNCLLanguageProfile

INCLUDEPATH += \
  src \
  $$PWD/../ncl-profile \
  $$PWD/../ncl-textual-view/src

HEADERS = \
  src/NCLTreeWidget.h \
  src/NCLParser.h \
  src/OutlineViewPlugin.h \
  src/OutlineViewFactory.h

SOURCES = \
  src/NCLTreeWidget.cpp \
  src/NCLParser.cpp \
  src/OutlineViewPlugin.cpp \
  src/OutlineViewFactory.cpp

RESOURCES += \
  ../common-resources/images.qrc \
  ../common-resources/language.qrc

OTHER_FILES += \
  outline_view.json
