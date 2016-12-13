cmake_minimum_required (VERSION 3.2)
project(debugconsole)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

include_directories(
  .
  "${CMAKE_SOURCE_DIR}/src/core/src"
  ${nclcomposer_core_SOURCE_DIR}
)

add_definitions(-DDEBUGCONSOLE_LIBRARY)

find_package (Qt5Core REQUIRED)
find_package (Qt5Xml REQUIRED)

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
)

install (TARGETS debugconsole DESTINATION plugins)
