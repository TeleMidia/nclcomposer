cmake_minimum_required (VERSION 3.2)
project(debugconsole)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

configure_file(debug_console.json debug_console.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(DEBUG_CONSOLE_SOURCES
  DebugConsoleFactory.cpp
  DebugConsoleFactory.h
  DebugConsolePlugin.cpp
  DebugConsolePlugin.h
  DebugConsole_global.h
)

add_library(debugconsole SHARED
  ${DEBUG_CONSOLE_SOURCES}
)

target_link_libraries (debugconsole
  nclcomposer_core
  Qt5::Widgets
)

target_include_directories (debugconsole
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

install (TARGETS debugconsole DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
