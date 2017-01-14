cmake_minimum_required (VERSION 3.2)
project(ncl_layout_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

configure_file(ncl-layout-view.json ncl-layout-view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(NCL_LAYOUT_VIEW_SOURCES
  src/LayoutViewPlugin.cpp
  src/LayoutViewFactory.cpp
  src/ui/view/LayoutView.cpp
  src/ui/view/LayoutRegion.cpp
  src/ui/view/LayoutRegionBase.cpp
  src/ui/view/LayoutGrid.cpp
  src/ui/view/LayoutCanvas.cpp
  src/ui/LayoutWindow.cpp

  src/LayoutViewPlugin.h
  src/LayoutViewFactory.h
  src/ui/view/LayoutCanvas.h
  src/ui/view/LayoutGrid.h
  src/ui/view/LayoutRegion.h
  src/ui/view/LayoutRegionBase.h
  src/ui/view/LayoutView.h
  src/ui/LayoutWindow.h

  src/ui/LayoutWindow.ui

  ncl-layout-view.qrc
)

add_library(ncl_layout_view SHARED
  ${NCL_LAYOUT_VIEW_SOURCES}
)

target_include_directories(ncl_layout_view PRIVATE
  .
  ui
  src
  src/ui
  src/ui/view
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ncl_layout_view
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_layout_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
