cmake_minimum_required (VERSION 3.5)

project(outline_view)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/plugins)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

add_definitions(-DNCLLANGUAGEPROFILE_LIBRARY)

include_directories(
  .
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

target_link_libraries (
  outline_view
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

