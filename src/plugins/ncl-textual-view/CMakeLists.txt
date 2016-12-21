cmake_minimum_required (VERSION 3.2)
project(ncl_textual_view)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file (ncl_textual_view.json ncl_textual_view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  src
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "../ncl-profile"
  "../outline-view/src"
  "deps/QScintilla-gpl-2.9/Qt4Qt5"
  "deps/QScintilla-gpl-2.9/include"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Widgets REQUIRED)
find_package (Qt5Xml REQUIRED)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/QScintilla-gpl-2.9/Qt4Qt5)

message(STATUS "Looking for qmake")
find_program(QMAKE_EXECUTABLE
  NAMES qmake qmake-qt4
  PATHS ENV QMAKE_PATH)
if (QMAKE_EXECUTABLE)
  set(QMAKE_FOUND 1)
  message(STATUS "Looking for qmake -- found")
endif()

add_custom_command(
  OUTPUT deps/QScintilla-gpl-2.9/Qt4Qt5/qscintila2_telem.dll
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/QScintilla-gpl-2.9/Qt4Qt5/
  COMMAND ${QMAKE_EXECUTABLE} FORCE_RELEASE=true
  COMMENT "Running qmake for qscintilla.pro"
  COMMAND make
  COMMENT "Building qscintilla2_telem.so"
)

set (NCL_TEXTUAL_VIEW_SOURCES
  src/NCLTextEditorMainWindow.cpp
  src/NCLTextEditor.cpp
  src/QsciLexerNCL.cpp
  src/QsciNCLAPIs.cpp
  src/MyLexer.cpp
  src/NCLValidator.cpp
  src/NCLProblemsView.cpp
  ../outline-view/src/NCLParser.cpp
  ../outline-view/src/NCLTreeWidget.cpp
  src/NCLTextualViewPlugin.cpp
  src/NCLTextualViewPluginFactory.cpp
  src/SearchLineEdit.cpp
  
  src/NCLTextEditorMainWindow.h
  src/NCLTextEditor.h
  src/QsciLexerNCL.h
  src/QsciNCLAPIs.h
  src/MyLexer.h
  src/NCLValidator.h
  src/NCLProblemsView.h
  ../outline-view/src/NCLParser.h
  ../outline-view/src/NCLTreeWidget.h
  src/NCLTextualViewPlugin.h
  src/NCLTextualViewPluginFactory.h
  src/NCLTextualViewPlugin_global.h
  src/SearchLineEdit.h
  
  deps/QScintilla-gpl-2.9/Qt4Qt5/qscintila2_telem.dll
)

qt5_wrap_ui (UI_GENERATED_HEADERS
  ui/TextualPluginPreferencesWidget.ui
)

add_library (ncl_textual_view
  SHARED
  ${NCL_TEXTUAL_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_link_libraries (ncl_textual_view
  qscintilla2_telem
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_textual_view DESTINATION plugins)
