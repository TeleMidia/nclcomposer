MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

TEMPLATE    =   app
TARGET      =   bin/ncl_textual_plugin
QT          +=  xml
CONFIG      +=  debug

DEFINES     +=  NCLEDITOR_STANDALONE

# We use QMAKE_CXXFLAGS instead of INCLUDEPATH because our qscintilla is
# modified, and must be found before any other that is installed.
QMAKE_CXXFLAGS  += -Ideps/QScintilla-gpl-2.6.3/Qt4Qt5

LIBS      +=    -Ldeps/QScintilla-gpl-2.6.3/Qt4Qt5 \
                -lqscintilla2_telem

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
                ../outline-view/include/NCLTreeWidget.h \
                include/SearchLineEdit.h


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
                ../outline-view/src/NCLTreeWidget.cpp \
                src/SearchLineEdit.cpp


OTHER_FILES +=  TODO \
                NEWS

RESOURCES   =   ../common-resources/language.qrc
