CONFIG += link_ncl_profile
include (../plugins-common.pri)

QT +=  xml gui

# Required to link against QSCINTILLA on Windows
DEFINES += QSCINTILLA_DLL

INCLUDEPATH += \
  src \
  ../outline-view/src \
  ../ncl-layout-view/src

# We use QMAKE_CXXFLAGS instead of INCLUDEPATH because our qscintilla is
# modified, and must be found before any other that is installed.
# If you know a better way to do that, please report us.
QMAKE_CXXFLAGS += -Ideps/QScintilla-gpl-2.8/Qt4Qt5 \
                  -Ideps/QScintilla-gpl-2.8/include

release {
LIBS      +=    -Ldeps/QScintilla-gpl-2.8/Qt4Qt5/release \
                -lqscintilla2_telem
}
else:debug {
LIBS      +=    -Ldeps/QScintilla-gpl-2.8/Qt4Qt5/debug \
                -lqscintilla2_telem
}

macx:LIBS += -L/Library/Frameworks \
            -L$$quote(/Library/Application Support/Composer/Extensions) \
            -lNCLLanguageProfile

unix:LIBS += -Ldeps/QScintilla-gpl-2.8/Qt4Qt5

win32:LIBS += -lNCLLanguageProfile

HEADERS     =   src/NCLTextEditorMainWindow.h \
                src/NCLTextEditor.h \
                src/QsciLexerNCL.h \
                src/QsciNCLAPIs.h \
                src/MyLexer.h \
                src/NCLValidator.h \
                src/NCLProblemsView.h \
                ../outline-view/src/NCLParser.h \
                ../outline-view/src/NCLTreeWidget.h \
                src/NCLTextualViewPlugin.h \
                src/NCLTextualViewPluginFactory.h \
                src/NCLTextualViewPlugin_global.h \
                src/SearchLineEdit.h

SOURCES     =   src/NCLTextEditorMainWindow.cpp \
                src/NCLTextEditor.cpp \
                src/QsciLexerNCL.cpp \
                src/QsciNCLAPIs.cpp \
                src/MyLexer.cpp \
                src/NCLValidator.cpp \
                src/NCLProblemsView.cpp \
                ../outline-view/src/NCLParser.cpp \
                ../outline-view/src/NCLTreeWidget.cpp \
                src/NCLTextualViewPlugin.cpp \
                src/NCLTextualViewPluginFactory.cpp \
                src/SearchLineEdit.cpp

OTHER_FILES +=  TODO \
                config/NCL_STRUCTURE \
                NEWS \
		ncl_textual_view.json

RESOURCES   =   ../common-resources/language.qrc
FORMS       +=  ui/TextualPluginPreferencesWidget.ui
