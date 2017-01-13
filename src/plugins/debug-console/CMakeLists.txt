cmake_minimum_required (VERSION 3.2)
project(debugconsole)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file (debug_console.json debug_console.json)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package (Qt5Core REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (DEBUG_CONSOLE_SOURCES
  src/DebugConsoleFactory.cpp
  src/DebugConsoleFactory.h
  src/DebugConsolePlugin.cpp
  src/DebugConsolePlugin.h
  src/DebugConsole_global.h
)

add_library(debugconsole
  SHARED
  ${DEBUG_CONSOLE_SOURCES}
)

target_link_libraries (debugconsole
  nclcomposer_core
  Qt5::Core
  Qt5::Widgets
)

install (TARGETS debugconsole DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
