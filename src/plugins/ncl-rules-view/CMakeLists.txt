cmake_minimum_required (VERSION 3.5)

project(ncl_rules_view)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/plugins)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

include_directories(
  .
  src
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-profile"
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "${CMAKE_BINARY_DIR}/src/plugins/ncl-structural-view"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Widgets REQUIRED)

set (NCL_RULES_VIEW_SOURCES
  src/ComboBoxItem.cpp
  src/CompositeRuleItem.cpp
  src/RuleItem.cpp
  src/RulesTreeWidget.cpp
  src/RulesView.cpp
  src/RulesViewFactory.cpp
  src/RulesViewPlugin.cpp
  
  src/Util.h
  src/RulesViewPlugin.h
  src/RulesViewFactory.h
  src/RulesView.h
  src/RulesTreeWidget.h
  src/RuleItem.h
  src/CompositeRuleItem.h
  src/ComboBoxItem.h
  
  ../common-resources/images.qrc
)

add_library (ncl_rules_view
  SHARED
  ${NCL_RULES_VIEW_SOURCES}
)

target_link_libraries (ncl_rules_view
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)
