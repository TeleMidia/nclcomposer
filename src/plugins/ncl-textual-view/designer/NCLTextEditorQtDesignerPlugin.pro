CONFIG      += designer plugin qt
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

CONFIG += release

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target

LIBS        +=  -lqscintilla2 \
                -L/usr/local/lib/composer/extension -lNCLLanguageProfile


SOURCES     +=  NCLTextEditorQtDesignerPlugin.cpp \
                ../src/NCLTextEditor.cpp \
                ../src/QsciLexerNCL.cpp \
                ../src/QsciNCLAPIs.cpp


HEADERS  +=     NCLTextEditorQtDesignerPlugin.h \
                ../include/NCLTextEditor.h \
                ../include/QsciLexerNCL.h \
                ../include/QsciNCLAPIs.h

INCLUDEPATH +=  ../include \
                /usr/local/include/composer \
                /usr/local/include/composer/core \
                /usr/local/include/composer/core/extensions

DBFILE       = plugin.db

RESOURCES   = ncl_text_editor.qrc

