cmake_minimum_required (VERSION 3.2)
project(outline_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(outline_view.json outline_view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(OUTLINE_VIEW_SOURCES
  src/NCLTreeWidget.h
  src/NCLParser.h
  src/OutlineViewPlugin.h
  src/OutlineViewFactory.h

  src/NCLTreeWidget.cpp
  src/NCLParser.cpp
  src/OutlineViewPlugin.cpp
  src/OutlineViewFactory.cpp
)

add_library (outline_view SHARED
  ${OUTLINE_VIEW_SOURCES}
)

target_include_directories (outline_view PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  src
)

target_link_libraries (outline_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install (TARGETS outline_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
