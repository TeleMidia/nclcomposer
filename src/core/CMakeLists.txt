cmake_minimum_required (VERSION 3.2)
project(nclcomposer_core)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(NCLCOMPOSER_CORE_HEADERS
# Modules
  modules/PluginControl.h
  modules/MessageControl.h
  modules/LanguageControl.h
  modules/ProjectControl.h
  modules/ProjectReader.h

# Extensions
  extensions/ILanguageProfile.h
  extensions/IDocumentParser.h
  extensions/IPlugin.h
  extensions/IPluginFactory.h

# Util
  util/ComposerCore_global.h
  util/Singleton.h
  util/AttributeReferences.h
  util/Utilities.h
  util/ComposerSettings.h
  util/Commands.h
  util/Preferences.h

# Model
  model/Entity.h
  model/Project.h
  #model/Model.h
  model/Exceptions.h
)

set(NCLCOMPOSER_CORE_SOURCES
  model/Entity.cpp
  model/Project.cpp
  modules/LanguageControl.cpp
  modules/MessageControl.cpp
  modules/PluginControl.cpp
  modules/ProjectControl.cpp
  modules/ProjectReader.cpp
  util/AttributeReferences.cpp
  util/Commands.cpp
  util/ComposerSettings.cpp
  util/Utilities.cpp
  util/Preferences.cpp
)

add_library(nclcomposer_core SHARED
  ${NCLCOMPOSER_CORE_SOURCES}
  ${NCLCOMPOSER_CORE_HEADERS}
)

target_compile_definitions(nclcomposer_core PRIVATE
  COMPOSERCORE_MAKE_DLL
)

target_include_directories(nclcomposer_core
  PUBLIC .
)

target_link_libraries(nclcomposer_core
  LINK_PUBLIC Qt5::Xml Qt5::Widgets
)

if(WIN32)
  install(TARGETS nclcomposer_core DESTINATION ${NCLCOMPOSER_BINARY_DIR_BIN})
else()
  install(TARGETS nclcomposer_core DESTINATION ${NCLCOMPOSER_BINARY_DIR_LIB})
endif()
