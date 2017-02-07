cmake_minimum_required (VERSION 3.2)
project(nclexport)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(nclexport.json nclexport.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(NCLEXPORT_SOURCES
  NCLExport_global.h

  NCLExportFactory.cpp
  NCLExportFactory.h
  NCLExportPlugin.cpp
  NCLExportPlugin.h
)

add_library(nclexport SHARED
  ${NCLEXPORT_SOURCES}
)

target_link_libraries(nclexport
  nclcomposer_core
  Qt5::Widgets
)

target_include_directories(nclexport
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

install(TARGETS nclexport DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})

