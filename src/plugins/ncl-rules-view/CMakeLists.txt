cmake_minimum_required (VERSION 3.2)
project(ncl_rules_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)

configure_file (ncl-rules-view.json ncl-rules-view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  .
  src
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "${CMAKE_SOURCE_DIR}/src/plugins/ncl-profile"
  ${nclcomposer_core_SOURCE_DIR}
)

find_package (Qt5Core REQUIRED)
find_package (Qt5Xml REQUIRED)
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
)

add_library (ncl_rules_view
  SHARED
  ${NCL_RULES_VIEW_SOURCES}
)

target_link_libraries (ncl_rules_view
  nclcomposer_core
  nclprofile
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
)

install (TARGETS ncl_rules_view DESTINATION ${NCLCOMPOSER_BINARY_DIR_PLUGINS})
