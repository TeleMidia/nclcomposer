cmake_minimum_required (VERSION 3.2)
project(ncl_layout_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

configure_file(ncl-layout-view.json.in ncl-layout-view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(NCL_LAYOUT_VIEW_SOURCES
  LayoutViewPlugin.cpp
  LayoutViewFactory.cpp
  ui/view/LayoutView.cpp
  ui/view/LayoutRegion.cpp
  ui/view/LayoutRegionBase.cpp
  ui/view/LayoutGrid.cpp
  ui/view/LayoutCanvas.cpp
  ui/LayoutWindow.cpp

  LayoutViewPlugin.h
  LayoutViewPlugin_global.h
  LayoutViewFactory.h
  ui/view/LayoutCanvas.h
  ui/view/LayoutGrid.h
  ui/view/LayoutRegion.h
  ui/view/LayoutRegionBase.h
  ui/view/LayoutView.h
  ui/LayoutWindow.h

  ui/LayoutWindow.ui
)

add_library(ncl_layout_view SHARED
  ${NCL_LAYOUT_VIEW_SOURCES}
)

target_include_directories(ncl_layout_view PRIVATE
  .
  ui
  ui/view
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ncl_layout_view
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_layout_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
