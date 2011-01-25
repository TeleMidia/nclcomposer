TEMPLATE    =   lib
TARGET      =   ncl_textual_plugin
QT          +=  xml
CONFIG      +=  debug plugin

INCLUDEPATH +=  include \
                ../ncl-layout-view/

macx {
    LIBS += -framework \
        ComposerCore
    INCLUDEPATH += /Library/Frameworks/ComposerCore.framework/
    DESTDIR = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += /usr/local/include/composer
    DESTDIR = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -LC:/composer/lib/composer \
        -lComposerCore1
    INCLUDEPATH += C:/composer/include/composer
    DESTDIR = $$quote(C:/composer/lib/composer/)
}


HEADERS     =   include/NCLTextEditorMainWindow.h \
                include/NCLTextEditor.h \
                include/QsciLexerNCL.h \
                include/QsciNCLAPIs.h \
                include/NCLStructure.h \
                include/MyLexer.h \
                include/NCLValidator.h \
                include/NCLParser.h \
                include/NCLProblemsView.h \
                include/NCLTreeWidget.h \
                include/NCLTextualViewPlugin.h \
                include/NCLTextualViewPluginFactory.h \
                include/NCLTextualViewPlugin_global.h

SOURCES     =   src/main.cpp \
                src/NCLTextEditorMainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                src/NCLStructure.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLParser.cpp \
                src/NCLProblemsView.cpp \
                src/NCLTreeWidget.cpp \
                src/NCLTextualViewPlugin.cpp \
                src/NCLTextualViewPluginFactory.cpp

OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS

RESOURCES   =   ncl_textual_plugin.qrc

LIBS        +=  -lqscintilla2

MOC_DIR     =   build
OBJECTS_DIR =   build

FORMS +=    ../ncl-layout-view/LayoutView.ui \
            ui/TextualPluginPreferencesWidget.ui
