cmake_minimum_required (VERSION 3.2)
project(ncl_textual_view)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(ncl_textual_view.json ncl_textual_view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set (NCL_TEXTUAL_VIEW_SOURCES
  src/NCLTextEditorMainWindow.cpp
  src/NCLTextEditor.cpp
  src/QsciLexerNCL.cpp
  src/QsciNCLAPIs.cpp
  src/MyLexer.cpp
  ../outline-view/src/NCLParser.cpp
  ../outline-view/src/NCLTreeWidget.cpp
  src/NCLTextualViewPlugin.cpp
  src/NCLTextualViewPluginFactory.cpp
  src/SearchLineEdit.cpp
  src/SearchBox.cpp
  
  src/NCLTextEditorMainWindow.h
  src/NCLTextEditor.h
  src/QsciLexerNCL.h
  src/QsciNCLAPIs.h
  src/MyLexer.h
  ../outline-view/src/NCLParser.h
  ../outline-view/src/NCLTreeWidget.h
  src/NCLTextualViewPlugin.h
  src/NCLTextualViewPluginFactory.h
  src/NCLTextualViewPlugin_global.h
  src/SearchLineEdit.h
  src/SearchBox.h
)

qt5_wrap_ui(UI_GENERATED_HEADERS
  ui/TextualPluginPreferencesWidget.ui
  src/SearchBox.ui
)

add_library(ncl_textual_view SHARED
  ${NCL_TEXTUAL_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_compile_definitions(ncl_textual_view
  PRIVATE QSCINTILLA_DLL
)

add_subdirectory(deps/QScintilla-gpl-2.9/Qt4Qt5)

target_include_directories(ncl_textual_view PRIVATE
  src
  "../outline-view/src"
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries (ncl_textual_view PRIVATE
  qscintilla2_telem
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_textual_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
