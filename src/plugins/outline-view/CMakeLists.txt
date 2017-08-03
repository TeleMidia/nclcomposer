cmake_minimum_required (VERSION 3.2)
project(outline_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(outline-view.json.in outline-view.json)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(OUTLINE_VIEW_SOURCES
  NCLTreeWidget.h
  NCLParser.h
  OutlineViewPlugin_global.h
  OutlineViewPlugin.h
  OutlineViewFactory.h

  NCLTreeWidget.cpp
  NCLParser.cpp
  OutlineViewPlugin.cpp
  OutlineViewFactory.cpp
)

add_library (outline_view SHARED
  ${OUTLINE_VIEW_SOURCES}
)

target_include_directories (outline_view PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  .
)

target_link_libraries (outline_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install (TARGETS outline_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
