cmake_minimum_required (VERSION 3.2)
project(outline_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file (outline_view.json outline_view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  src
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-profile"
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-textual-view/src"
  "${CMAKE_SOURCE_DIR}/src/core/src"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Widgets REQUIRED)

set (OUTLINE_VIEW_SOURCES
  src/NCLTreeWidget.h
  src/NCLParser.h
  src/OutlineViewPlugin.h
  src/OutlineViewFactory.h

  src/NCLTreeWidget.cpp
  src/NCLParser.cpp
  src/OutlineViewPlugin.cpp
  src/OutlineViewFactory.cpp
)

add_library(outline_view
  SHARED
  ${OUTLINE_VIEW_SOURCES}
)

target_link_libraries (outline_view
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install (TARGETS outline_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
