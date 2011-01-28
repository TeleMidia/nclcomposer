include (../plugins-common.pri)
TARGET          = outline_view
QT              += xml

INCLUDEPATH     +=  include

HEADERS         =   include/NCLTreeWidget.h \
                    include/NCLParser.h \
                    include/OutlineViewPlugin.h \
                    include/OutlineViewFactory.h

SOURCES         =   src/NCLTreeWidget.cpp \
                    src/NCLParser.cpp \
                    src/OutlineViewPlugin.cpp \
                    src/OutlineViewFactory.cpp

INSTALLS        =   target
