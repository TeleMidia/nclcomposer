TEMPLATE    =   app
TARGET      =   bin/ncl_textual_plugin
QT          +=  xml
CONFIG      +=  release

INCLUDEPATH +=  include \
      ../layoutView/

HEADERS     =   include/MainWindow.h \
                include/NCLTextEditor.h \
                include/QsciLexerNCL.h \
                include/QsciNCLAPIs.h \
                include/NCLStructure.h \
                include/MyLexer.h \
                include/NCLValidator.h \
                include/NCLParser.h \
                include/NCLProblemsView.h \
                include/NCLTreeWidget.h \
    ../layoutView/scribblearea.h \
    ../layoutView/nclregion.h \
    ../layoutView/LayoutView.h \
    ../layoutView/attributes.h

SOURCES     =   src/main.cpp \
                src/MainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                src/NCLStructure.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLParser.cpp \
                src/NCLProblemsView.cpp \
                src/NCLTreeWidget.cpp \
    ../layoutView/scribblearea.cpp \
    ../layoutView/nclregion.cpp \
    ../layoutView/LayoutView.cpp \
    ../layoutView/attributes.cpp

OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS

RESOURCES   =   ncl_textual_plugin.qrc

LIBS        +=  -lqscintilla2

MOC_DIR     =   build
OBJECTS_DIR =   build

FORMS += \
    ../layoutView/LayoutView.ui
