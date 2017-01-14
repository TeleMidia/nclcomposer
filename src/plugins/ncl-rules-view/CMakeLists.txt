cmake_minimum_required (VERSION 3.2)
project(ncl_rules_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file(ncl-rules-view.json ncl-rules-view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(NCL_RULES_VIEW_SOURCES
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
)

add_library(ncl_rules_view SHARED
  ${NCL_RULES_VIEW_SOURCES}
)

target_include_directories(ncl_rules_view PRIVATE
  .
  src
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ncl_rules_view PRIVATE
  nclcomposer_core
  nclprofile
  Qt5::Widgets
)

install (TARGETS ncl_rules_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
