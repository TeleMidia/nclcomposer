CONFIG      += designer plugin
TARGET      = $$qtLibraryTarget($$TARGET)
TEMPLATE    = lib

CONFIG += release

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS += target

LIBS        +=  -lqscintilla2


SOURCES     +=  NCLTextEditorPlugin.cpp \
                ../src/NCLTextEditor.cpp \
                ../src/QsciLexerNCL.cpp \
                ../src/QsciNCLAPIs.cpp \
                ../src/NCLStructure.cpp


HEADERS  +=     NCLTextEditorPlugin.h \
                ../include/NCLTextEditor.h \
                ../include/QsciLexerNCL.h \
                ../include/QsciNCLAPIs.h \
                ../include/NCLStructure.h

INCLUDEPATH += ../

DBFILE       = plugin.db

OTHER_FILES += \
    config/apiNCL.txt \
    TODO \
    config/NCL_STRUCTURE
