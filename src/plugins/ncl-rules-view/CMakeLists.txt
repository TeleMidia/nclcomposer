cmake_minimum_required (VERSION 3.2)
project(ncl_rules_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file(ncl-rules-view.json.in ncl-rules-view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(NCL_RULES_VIEW_SOURCES
  ComboBoxItem.cpp
  CompositeRuleItem.cpp
  RuleItem.cpp
  RulesTreeWidget.cpp
  RulesViewPlugin.cpp
  
  Util.h
  RulesViewPlugin.h
  RulesTreeWidget.h
  RuleItem.h
  CompositeRuleItem.h
  ComboBoxItem.h
)

add_library(ncl_rules_view SHARED
  ${NCL_RULES_VIEW_SOURCES}
)

target_include_directories(ncl_rules_view PRIVATE
  .
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ncl_rules_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install (TARGETS ncl_rules_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})

