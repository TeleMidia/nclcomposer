cmake_minimum_required (VERSION 3.2)
project(properties_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file (properties_view.json properties_view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  src
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-profile"
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "${CMAKE_BINARY_DIR}/src/plugins/properties-view"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Xml REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (PROPERTIES_VIEW_SOURCES
  src/PropertiesViewFactory.cpp
  src/PropertiesViewPlugin.cpp
  src/PropertyEditor.cpp
  src/QLLineEditFilter.cpp
  src/LineEditWithButton.cpp
  src/PropertyButton.cpp

  src/PropertiesViewFactory.h
  src/PropertiesView_global.h
  src/PropertyEditor.h
  src/PropertiesViewPlugin.h
  src/QLineEditFilter.h
  src/LineEditWithButton.h
  src/PropertyButton.h
)

qt5_wrap_ui (UI_GENERATED_HEADERS
  ui/PropertyEditorWidget.ui
)

add_library (properties_view
  SHARED
  ${PROPERTIES_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_link_libraries (properties_view
  nclcomposer_core
  nclprofile
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS properties_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
