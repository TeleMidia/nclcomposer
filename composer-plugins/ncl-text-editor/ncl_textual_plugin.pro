TEMPLATE    =   app
TARGET      =   bin/ncl_textual_plugin
QT          +=  xml
CONFIG      +=  release

INCLUDEPATH +=  include

HEADERS     =   include/MainWindow.h \
                include/NCLTextEditor.h \
                include/QsciLexerNCL.h \
                include/QsciNCLAPIs.h \
                include/NCLStructure.h \
                include/MyLexer.h \
                include/NCLValidator.h \
                include/NCLParser.h \
                include/NCLTreeWidget.h

SOURCES     =   src/main.cpp \
                src/MainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                src/NCLStructure.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLParser.cpp \
                src/NCLTreeWidget.cpp

OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS

RESOURCES   =   ncl_textual_plugin.qrc

LIBS        +=  -lqscintilla2

MOC_DIR     =   build
OBJECTS_DIR =   build
