cmake_minimum_required (VERSION 3.2)
project(properties_view)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(properties_view.json properties_view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PROPERTIES_VIEW_SOURCES
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

qt5_wrap_ui(UI_GENERATED_HEADERS
  ui/PropertyEditorWidget.ui
)

add_library(properties_view SHARED
  ${PROPERTIES_VIEW_SOURCES}
  ${UI_GENERATED_HEADERS}
)

target_include_directories(properties_view PRIVATE
  src
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(properties_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install(TARGETS properties_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
