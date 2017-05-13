cmake_minimum_required (VERSION 3.2)
project(validator)

set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

configure_file(validator-plugin.json.in validator-plugin.json)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets)

set(VALIDATOR_SOURCES
  ValidatorPlugin.cpp
  ValidatorFactory.cpp
  langstruct/langstruct.cpp
  langstruct/components/ReferenceStructure.cpp
  langstruct/components/ElementStructure.cpp
  langstruct/components/AttributeStructure.cpp
  message/message.cpp
  nclmodel/nclmodel.cpp
  validation/Validator.cpp
  composerncladapter.cpp
  validation/semanticvalidation.cpp
  validation/structuralvalidation.cpp
  validatortreewidgetitem.cpp
  nclmodel/textualparser.cpp
  nclmodel/connectorparser.cpp

  ValidatorPlugin.h
  ValidatorFactory.h
  definitions/definitions.h
  langstruct/langstruct.h
  langstruct/components/ReferenceStructure.h
  langstruct/components/ElementStructure.h
  langstruct/components/AttributeStructure.h
  message/message.h
  nclmodel/nclmodel.h
  validation/Validator.h
  composerncladapter.h
  validation/semanticvalidation.h
  validation/structuralvalidation.h
  validatortreewidgetitem.h
  nclmodel/textualparser.h
  nclmodel/connectorparser.h
  
  Resource.qrc
)

add_library(validator_plugin SHARED
  ${VALIDATOR_SOURCES}
)

target_include_directories(validator_plugin
  PRIVATE . ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(validator_plugin PRIVATE
  nclcomposer_core
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS validator_plugin
         DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})

option(WITH_VALIDATOR_EXE "Build the ncl-validator command-line" OFF)

if (WITH_VALIDATOR_EXE)
  add_executable (nclvalidator main.cpp
    ${VALIDATOR_SOURCES}
  )

  target_include_directories(nclvalidator
    PRIVATE . ${CMAKE_CURRENT_BINARY_DIR}
  )

  # TODO: We probably do not need those libraries
  target_link_libraries(nclvalidator PRIVATE
    nclcomposer_core
    Qt5::Xml
    Qt5::Widgets
  )

  message(STATUS "Build ncl-validator executable.")
else()
  message(STATUS "ncl-validator executable WILL NOT BE built.")
endif()

