cmake_minimum_required (VERSION 3.2)
project(nclcomposer_gui)

option(cpr_with_fervor "Use Fervor update system" OFF)
if(cpr_with_fervor)
  message("Use fervor gui")
  add_definitions(-DFV_GUI)
else()
  message("Do not use fervor gui")
endif()

# Current date and time
string(TIMESTAMP BUILD_DATE "%Y-%m-%d %H:%M")
add_definitions(-DBUILD_DATE="${BUILD_DATE}")

# Definitions
add_definitions(-DNCLCOMPOSER_GUI_VERSION="${VERSION}")
add_definitions(-DNCL_COMPOSER_NOTIFY_URL="http://composer.telemidia.puc-rio.br/")
add_definitions(-DDEFAULT_MIN_MESSAGE_ID_TO_SHOW=1)
add_definitions(-DDEFAULT_MAX_NOTIFY_MESSAGES=3)
add_definitions(-DQTOOLWINDOWMANAGER_BUILD_LIB)
add_definitions(-DDATA_PATH="~/")

# CMake/Qt Integration
set(CMAKE_INCLUDE_CURRENTDIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Xml Widgets Network)
find_library(nclcomposer_core REQUIRED)

set(UI_SOURCES
  ui/PreferencesDialog.ui
  ui/ComposerMainWindow.ui
  ui/PerspectiveManager.ui
  ui/RunGingaConfig.ui
  ui/PluginDetailsDialog.ui
  ui/EnvironmentPreferencesWidget.ui
  ui/WelcomeWidget.ui
  ui/AboutDialog.ui
  ui/ShortcutsDialog.ui
  ui/GeneralPreferences.ui
  ui/NewProjectWizard.ui
  ui/FileChooser.ui
)

QT5_WRAP_UI(UI_GENERATED_HEADERS
  ${UI_SOURCES}
)

add_executable(nclcomposer
  main.cpp
  src/ComposerMainWindow.cpp
  src/PreferencesDialog.cpp
  src/PerspectiveManager.cpp
  src/EnvironmentPreferencesWidget.cpp
  src/WelcomeWidget.cpp
  src/AboutDialogs.cpp
  src/RunGingaConfig.cpp
  src/GeneralPreferences.cpp
  src/NewProjectWizard.cpp
  src/FileChooser.cpp
  src/ComposerSplashScreen.cpp

  src/toolwindowmanager/libqtoolwindowmanager/qabstracttoolwindowmanagerarea.cpp
  src/toolwindowmanager/libqtoolwindowmanager/qabstracttoolwindowmanagerarea.h
  src/toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanagerarea.cpp
  src/toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanager.cpp
  src/toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanager.h
  src/toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanagerwrapper.cpp
  src/toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanagerarea_p.h
  src/toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanager_p.h
  src/toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanagerwrapper_p.h

  ${UI_GENERATED_HEADERS}
  gui.qrc
  ../plugins/common-resources/images.qrc
  ../plugins/common-resources/language.qrc
)

set_target_properties(nclcomposer PROPERTIES
  BUILD_WITH_INSTALL_RPATH TRUE
  INSTALL_RPATH "\$ORIGIN/../lib/${CMAKE_PROJECT_NAME}:..."
)

target_link_libraries(nclcomposer
  nclcomposer_core
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
  Qt5::Network
)

target_include_directories(nclcomposer PRIVATE
  src
  "${CMAKE_BINARY_DIR}/src/gui"
  "src/toolwindowmanager/libqtoolwindowmanager"
)

set(GUI_DATAFILES
 data/defaultConnBase.ncl
 data/style.qss
 data/style.ini
 data/default.ini
)

# Copy datafiles to build dir
add_custom_target(copy_datafiles ALL)
foreach(file_i ${GUI_DATAFILES})
  get_filename_component(filename ${file_i} NAME)
  add_custom_command(TARGET copy_datafiles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/${file_i}
      ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_DATA_DIR}/${filename}
    DEPENDS ${file_i}
    COMMENT "Copying ${file_i}")
endforeach(file_i)

install(TARGETS nclcomposer DESTINATION bin)
install(FILES ${GUI_DATAFILES} DESTINATION ${NCLCOMPOSER_DATA_DIR})
