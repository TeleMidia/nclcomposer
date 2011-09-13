MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

TEMPLATE    =   app
TARGET      =   bin/ncl_textual_plugin
QT          +=  xml
CONFIG      +=  debug

DEFINES     +=  NCLEDITOR_STANDALONE=\"1\"

LIBS        += -Ldeps/QScintilla-gpl-2.5.1/Qt4 -lqscintilla2

INCLUDEPATH +=  include \
                ../outline-view/include \
                ../ncl-layout-view \
                ../../composer-core/core/include \
                ../../composer-core/extensions/NCL

HEADERS     =   include/NCLTextEditorMainWindow.h \
                include/NCLTextEditor.h \
                include/QsciLexerNCL.h \
                include/QsciNCLAPIs.h \
                ../../composer-core/core/include/util/AttributeReferences.h \
                ../../composer-core/extensions/NCL/NCLStructure.h \
                include/MyLexer.h \
                include/NCLValidator.h \
                include/NCLProblemsView.h \
                ../outline-view/include/NCLParser.h \
                ../outline-view/include/NCLTreeWidget.h


SOURCES     =   src/main.cpp \
                src/NCLTextEditorMainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                ../../composer-core/core/src/util/AttributeReferences.cpp \
                ../../composer-core/extensions/NCL/NCLStructure.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLProblemsView.cpp \
                ../outline-view/src/NCLParser.cpp \
                ../outline-view/src/NCLTreeWidget.cpp


OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS

RESOURCES   =   ncl_textual_plugin.qrc
