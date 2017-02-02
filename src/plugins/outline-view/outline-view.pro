CONFIG += link_ncl_profile
include (../plugins-common.pri)

QT              += xml

win32:LIBS += -lNCLLanguageProfile

INCLUDEPATH += \
  $$PWD \
  $$PWD/../ncl-profile \
  $$PWD/../ncl-textual-view/src

HEADERS = \
  NCLTreeWidget.h \
  NCLParser.h \
  OutlineViewPlugin.h \
  OutlineViewFactory.h

SOURCES = \
  NCLTreeWidget.cpp \
  NCLParser.cpp \
  OutlineViewPlugin.cpp \
  OutlineViewFactory.cpp

RESOURCES += \
  ../common-resources/images.qrc \
  ../common-resources/language.qrc

OTHER_FILES += \
  outline_view.json

