cmake_minimum_required (VERSION 3.2)
project(ncl_structural_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

configure_file(ncl-structural-view.json.in ncl-structural-view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(NCL_STRUCTURAL_VIEW_SOURCES
  Structural.h
  StructuralUtil.h
  StructuralViewPlugin.h
  StructuralViewFactory.h
  ui/Commands.h
  ui/StructuralWindow.h
  ui/view/StructuralView.h
  ui/view/StructuralScene.h
  ui/view/StructuralEdge.h
  ui/view/StructuralNode.h
  ui/view/StructuralMinimap.h
  ui/view/StructuralMenu.h
  ui/view/StructuralInterface.h
  ui/view/StructuralLink.h
  ui/view/StructuralBind.h 
  ui/view/StructuralEntity.h
  ui/view/StructuralComposition.h
  ui/view/StructuralContent.h
  ui/view/StructuralLinkDialog.h
  ui/view/StructuralLinkTool.h

  StructuralUtil.cpp
  StructuralViewPlugin.cpp
  ui/Commands.cpp
  ui/StructuralWindow.cpp
  ui/view/StructuralView.cpp
  ui/view/StructuralScene.cpp
  ui/view/StructuralNode.cpp
  ui/view/StructuralMinimap.cpp
  ui/view/StructuralEdge.cpp
  ui/view/StructuralMenu.cpp
  ui/view/StructuralInterface.cpp
  ui/view/StructuralEntity.cpp
  ui/view/StructuralLink.cpp
  ui/view/StructuralBind.cpp
  ui/view/StructuralComposition.cpp
  ui/view/StructuralContent.cpp
  ui/view/StructuralLinkDialog.cpp
  ui/view/StructuralLinkTool.cpp
)

option(WITH_STRUCTURAL_AUTO_ADJUST "Build 'Structural View' with 'Auto Adjust' feature (require GraphViz)." OFF)

if (WITH_STRUCTURAL_AUTO_ADJUST)
  # Cheking
  find_library(GRAPHVIZ_LIBS NAMES gvc cgraph)

  if(NOT GRAPHVIZ_LIBS)
    message(FATAL_ERROR "[ ! ] GraphViz library not found!")
  endif()

  # Set WITH_GRAPHVIZ
  add_definitions(-DWITH_GRAPHVIZ)

  # Update DEPS_LIBS
  set(DEPS_LIBS ${DEPS_LIBS} ${GRAPHVIZ_LIBS})
endif()

add_library(ncl_structural_view
  SHARED
  ${NCL_STRUCTURAL_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_include_directories(ncl_structural_view PRIVATE
  .
  ui
  src
  src/core
  ui
  ui/view
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ncl_structural_view PRIVATE
  nclcomposer_core
  nclprofile
  ${DEPS_LIBS}
  Qt5::Xml
  Qt5::Widgets
)

install(TARGETS ncl_structural_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
