cmake_minimum_required (VERSION 3.2)
project(nclcomposer_gui)

option(cpr_with_fervor "Use Fervor update system" OFF)
if(cpr_with_fervor)
  message(STATUS "Use fervor gui")
  add_definitions(-DFV_GUI)
else()
  message(STATUS "Do not use fervor gui")
endif()

# Set some Win32 Specific Settings
IF(WIN32)
SET(GUI_TYPE WIN32)
ENDIF(WIN32)

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
  PreferencesDialog.ui
  ComposerMainWindow.ui
  PerspectiveManager.ui
  RunGingaConfig.ui
  PluginDetailsDialog.ui
  EnvironmentPreferencesWidget.ui
  WelcomeWidget.ui
  AboutDialog.ui
  ShortcutsDialog.ui
  GeneralPreferences.ui
  NewProjectWizard.ui
  FileChooser.ui
  QSimpleUpdater/src/Downloader.ui
)

QT5_WRAP_UI(UI_GENERATED_HEADERS
  ${UI_SOURCES}
)

#add_library(QSimpleUpdater INTERFACE)
#target_include_directories(QSimpleUpdater INTERFACE)
#target_sources(QSimpleUpdater INTERFACE
#  ${CMAKE_CURRENT_SOURCE_DIR}/src/QsimpleUpdater/include/QsimpleUpdater.h
#)

add_executable(nclcomposer ${GUI_TYPE}
  main.cpp
  ComposerMainWindow.cpp
  PreferencesDialog.cpp
  PerspectiveManager.cpp
  EnvironmentPreferencesWidget.cpp
  WelcomeWidget.cpp
  AboutDialogs.cpp
  RunGingaConfig.cpp
  GeneralPreferences.cpp
  NewProjectWizard.cpp
  FileChooser.cpp
  ComposerSplashScreen.cpp

  toolwindowmanager/libqtoolwindowmanager/qabstracttoolwindowmanagerarea.cpp
  toolwindowmanager/libqtoolwindowmanager/qabstracttoolwindowmanagerarea.h
  toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanagerarea.cpp
  toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanager.cpp
  toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanager.h
  toolwindowmanager/libqtoolwindowmanager/qtoolwindowmanagerwrapper.cpp
  toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanagerarea_p.h
  toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanager_p.h
  toolwindowmanager/libqtoolwindowmanager/private/qtoolwindowmanagerwrapper_p.h

  QSimpleUpdater/src/Downloader.cpp
  QSimpleUpdater/src/QSimpleUpdater.cpp
  QSimpleUpdater/include/QSimpleUpdater.h
  QSimpleUpdater/src/Updater.cpp  

  ${UI_GENERATED_HEADERS}
  gui.qrc
  ../plugins/common-resources/images.qrc
  ../plugins/common-resources/language.qrc
  QSimpleUpdater/etc/resources/qsimpleupdater.qrc
  
  nclcomposer.rc
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
  ${CMAKE_CURRENT_SOURCE_DIR}
  "${CMAKE_BINARY_DIR}/src/gui"
  "toolwindowmanager/libqtoolwindowmanager"
  "QSimpleUpdater/"
  "QSimpleUpdater/include/"
  "src/QSimpleUpdater/src/"
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

