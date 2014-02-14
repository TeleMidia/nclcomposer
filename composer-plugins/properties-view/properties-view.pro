include (../plugins-common.pri)

INCLUDEPATH += \
  src

LIBS += \
  -lNCLLanguageProfile

DEFINES += \
  PROPERTIESVIEW_LIBRARY

SOURCES +=  \
  src/PropertiesViewFactory.cpp \
  src/PropertiesViewPlugin.cpp \
  src/PropertyEditor.cpp \
  src/QLLineEditFilter.cpp \
  src/LineEditWithButton.cpp \
  src/PropertyButton.cpp

HEADERS +=  \
  src/PropertiesViewFactory.h\
  src/PropertiesView_global.h \
  src/PropertyEditor.h \
  src/PropertiesViewPlugin.h \
  src/QLineEditFilter.h \
  src/LineEditWithButton.h \
  src/PropertyButton.h

FORMS += \
  ui/PropertyEditorWidget.ui

RESOURCES += \
  ../common-resources/language.qrc

OTHER_FILES += \
  properties_view.json
