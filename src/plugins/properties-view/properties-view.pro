CONFIG += link_ncl_profile
include (../plugins-common.pri)

INCLUDEPATH += \
  .

DEFINES += \
  PROPERTIESVIEW_LIBRARY

SOURCES +=  \
  PropertiesViewFactory.cpp \
  PropertiesViewPlugin.cpp \
  PropertyEditor.cpp \
  QLLineEditFilter.cpp \
  LineEditWithButton.cpp \
  PropertyButton.cpp

HEADERS +=  \
  PropertiesViewFactory.h\
  PropertiesView_global.h \
  PropertyEditor.h \
  PropertiesViewPlugin.h \
  QLineEditFilter.h \
  LineEditWithButton.h \
  PropertyButton.h

FORMS += \
  PropertyEditorWidget.ui

RESOURCES += \
  ../common-resources/language.qrc

OTHER_FILES += \
  properties_view.json

