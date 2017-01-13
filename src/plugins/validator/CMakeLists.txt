cmake_minimum_required (VERSION 3.2)
project(validator)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

configure_file (validator_plugin.json validator_plugin.json)

include_directories(
  .
  ${CMAKE_CURRENT_BINARY_DIR}
)

find_package (Qt5Core REQUIRED)
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Xml REQUIRED)

set (VALIDATOR_SOURCES
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
  main.cpp
  
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

add_library(validator
  SHARED
  ${VALIDATOR_SOURCES}
)

target_link_libraries(validator
  nclcomposer_core
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS validator DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
