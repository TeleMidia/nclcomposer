cmake_minimum_required (VERSION 3.2)
project(nclprofile)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

include_directories(
  .
  "${CMAKE_SOURCE_DIR}/src/core/src"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Core REQUIRED)
find_package (Qt5Xml REQUIRED)
find_package (Qt5Widgets REQUIRED)

set (NCL_PROFILE_SOURCES
  NCLDocumentParser.cpp
  NCLDocumentParser.h
  NCLLanguageProfile.cpp
  NCLLanguageProfile_global.h
  NCLLanguageProfile.h
  NCLStructure.cpp
  NCLStructure.h
)

add_library(nclprofile
  SHARED
  ${NCL_PROFILE_SOURCES}
  ../common-resources/language.qrc
)

target_compile_definitions(nclprofile PRIVATE
  NCLLANGUAGEPROFILE_MAKE_DLL
)

target_link_libraries (nclprofile
  nclcomposer_core
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS nclprofile DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
