cmake_minimum_required (VERSION 3.2)
project(ncl_rules_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

configure_file (ncl-rules-view.json ncl-rules-view.json)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
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

install (TARGETS ncl_rules_view DESTINATION plugins)
