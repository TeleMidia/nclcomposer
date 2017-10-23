cmake_minimum_required (VERSION 3.2)
project(ncl_structural_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

configure_file(ncl-structural-view.json.in ncl-structural-view.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(NCL_STRUCTURAL_VIEW_SOURCES
  src/core/Command.h
  src/core/Structural.h
  src/core/StructuralUtil.h
  src/StructuralViewPlugin.h
  src/StructuralViewFactory.h
  src/ui/StructuralWindow.h
  src/ui/view/StructuralView.h
  src/ui/view/StructuralScene.h
  src/ui/view/StructuralEdge.h
  src/ui/view/StructuralNode.h
  src/ui/view/StructuralMinimap.h
  src/ui/view/StructuralMenu.h
  src/ui/view/StructuralInterface.h
  src/ui/view/StructuralLink.h
  src/ui/view/StructuralBind.h 
  src/ui/view/StructuralEntity.h
  src/ui/view/StructuralComposition.h
  src/ui/view/StructuralContent.h
  src/ui/view/StructuralLinkDialog.h
  src/ui/view/StructuralLinkTool.h

  src/core/Command.cpp
  src/core/StructuralUtil.cpp
  src/StructuralViewPlugin.cpp
  src/StructuralViewFactory.cpp
  src/ui/StructuralWindow.cpp
  src/ui/view/StructuralView.cpp
  src/ui/view/StructuralScene.cpp
  src/ui/view/StructuralNode.cpp
  src/ui/view/StructuralMinimap.cpp
  src/ui/view/StructuralEdge.cpp
  src/ui/view/StructuralMenu.cpp
  src/ui/view/StructuralInterface.cpp
  src/ui/view/StructuralEntity.cpp
  src/ui/view/StructuralLink.cpp
  src/ui/view/StructuralBind.cpp
  src/ui/view/StructuralComposition.cpp
  src/ui/view/StructuralContent.cpp
  src/ui/view/StructuralLinkDialog.cpp
  src/ui/view/StructuralLinkTool.cpp
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
  src/ui
  src/ui/view
  src/util
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
