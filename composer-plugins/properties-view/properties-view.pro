TARGET = properties_view

include (../plugins-common.pri)

LIBS += -lNCLLanguageProfile

DEFINES += PROPERTIESVIEW_LIBRARY

INCLUDEPATH += include

SOURCES +=  src/PropertiesViewFactory.cpp \
            src/PropertiesViewPlugin.cpp \
            src/PropertyEditor.cpp \
            src/QLLineEditFilter.cpp \
    src/propertybuttons.cpp

HEADERS +=  include/PropertiesViewFactory.h\
            include/PropertiesView_global.h \
            include/PropertyEditor.h \
            include/PropertiesViewPlugin.h \
            include/QLineEditFilter.h \
    include/propertybuttons.h

#RESOURCES += images.qrc

INSTALLS += target

FORMS += \
    ui/PropertyEditorWidget.ui

RESOURCES += \
    ../common-resources/language.qrc
