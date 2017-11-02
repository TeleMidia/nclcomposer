cmake_minimum_required (VERSION 3.2)
project(ncl_textual_view)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(ncl-textual-view.json.in ncl-textual-view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set (NCL_TEXTUAL_VIEW_SOURCES
  NCLTextEditorMainWindow.cpp
  NCLTextEditor.cpp
  QsciLexerNCL.cpp
  QsciNCLAPIs.cpp
  MyLexer.cpp
  ../outline-view/NCLParser.cpp
  ../outline-view/NCLTreeWidget.cpp
  NCLTextualViewPlugin.cpp
  NCLTextualViewPluginFactory.cpp
  SearchLineEdit.cpp
  SearchBox.cpp
  
  NCLTextEditorMainWindow.h
  NCLTextEditor.h
  QsciLexerNCL.h
  QsciNCLAPIs.h
  MyLexer.h
  ../outline-view/NCLParser.h
  ../outline-view/NCLTreeWidget.h
  NCLTextualViewPlugin.h
  NCLTextualViewPluginFactory.h
  NCLTextualViewPlugin_global.h
  SearchLineEdit.h
  SearchBox.h
)

qt5_wrap_ui(UI_GENERATED_HEADERS
  TextualPluginPreferencesWidget.ui
  SearchBox.ui
)

add_library(ncl_textual_view SHARED
  ${NCL_TEXTUAL_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_compile_definitions(ncl_textual_view
  PRIVATE QSCINTILLA_DLL
)

add_subdirectory(deps/QScintilla_gpl-2.10.1/Qt4Qt5)

target_include_directories(ncl_textual_view PRIVATE
  .
  "../outline-view/"
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
