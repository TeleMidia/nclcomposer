TARGET = properties_view

include (../plugins-common.pri)

DEFINES += PROPERTIESVIEW_LIBRARY

INCLUDEPATH += include

SOURCES +=  src/PropertiesViewFactory.cpp \
            src/PropertiesViewPlugin.cpp \
            src/PropertyEditor.cpp \
    src/QLLineEditFilter.cpp

HEADERS +=  include/PropertiesViewFactory.h\
            include/PropertiesView_global.h \
            include/PropertyEditor.h \
            include/PropertiesViewPlugin.h \
    include/QLineEditFilter.h

#RESOURCES += images.qrc

INSTALLS += target

FORMS += \
    ui/PropertyEditorWidget.ui
