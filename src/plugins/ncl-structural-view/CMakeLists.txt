cmake_minimum_required (VERSION 3.2)
project(ncl_structural_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

configure_file (ncl-structural-view.json ncl-structural-view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  .
  src
  src/core
  src/ui
  src/ui/view
  src/util
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-profile"
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "${CMAKE_BINARY_DIR}/src/plugins/ncl-structural-view"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Xml REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (NCL_STRUCTURAL_VIEW_SOURCES
  src/core/Command.h
  src/core/Structural.h
  src/core/Remove.h
  src/core/Insert.h
  src/core/Change.h
  src/StructuralViewPlugin.h
  src/StructuralViewFactory.h
  src/ui/StructuralWindow.h
  src/ui/view/StructuralView.h
  src/util/StructuralUtil.h
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
  src/StructuralViewPlugin.cpp
  src/StructuralViewFactory.cpp
  src/ui/StructuralWindow.cpp
  src/ui/view/StructuralView.cpp
  src/util/StructuralUtil.cpp
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
  src/core/Remove.cpp
  src/core/Insert.cpp
  src/core/Change.cpp
  src/ui/view/StructuralContent.cpp
  src/ui/view/StructuralLinkDialog.cpp
  src/ui/view/StructuralLinkTool.cpp
  
  ncl-structural-view.qrc
  ../common-resources/images.qrc
)

qt5_wrap_ui (UI_GENERATED_HEADERS
  ui/StructuralLinkForm.ui
)

add_library (ncl_structural_view
  SHARED
  ${NCL_STRUCTURAL_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_link_libraries (ncl_structural_view
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_structural_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
