cmake_minimum_required (VERSION 3.2)
project(properties_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(properties-view.json.in properties-view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PROPERTIES_VIEW_SOURCES
  PropertiesViewFactory.cpp
  PropertiesViewPlugin.cpp
  PropertyEditor.cpp
  QLLineEditFilter.cpp
  LineEditWithButton.cpp
  PropertyButton.cpp

  PropertiesViewFactory.h
  PropertiesView_global.h
  PropertyEditor.h
  PropertiesViewPlugin.h
  QLineEditFilter.h
  LineEditWithButton.h
  PropertyButton.h
)

qt5_wrap_ui(UI_GENERATED_HEADERS
  PropertyEditorWidget.ui
)

add_library(properties_view SHARED
  ${PROPERTIES_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_include_directories(properties_view PRIVATE
  .
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(properties_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install(TARGETS properties_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
