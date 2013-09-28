include (../plugins-common.pri)
TARGET          = outline_view
QT              += xml

macx:LIBS +=    -L$$quote(/Library/Application Support/Composer/Extensions) \
                -lNCLLanguageProfile

win32:LIBS +=   -lNCLLanguageProfile

INCLUDEPATH     +=  include \
                    ../ncl-text-editor/include \

HEADERS         =   include/NCLTreeWidget.h \
                    include/NCLParser.h \
                    include/OutlineViewPlugin.h \
                    include/OutlineViewFactory.h

SOURCES         =   src/NCLTreeWidget.cpp \
                    src/NCLParser.cpp \
                    src/OutlineViewPlugin.cpp \
                    src/OutlineViewFactory.cpp

INSTALLS        =   target

RESOURCES += \
    ../common-resources/images.qrc \
    ../common-resources/language.qrc

OTHER_FILES += \
    outline_view.json
