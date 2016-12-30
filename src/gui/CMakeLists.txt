cmake_minimum_required (VERSION 3.2)
project(nclcomposer_gui)

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
set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5Xml REQUIRED)
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Network REQUIRED)
#find_library (sass_aaa OPTIONAL)
find_library (nclcomposer_core REQUIRED)

include_directories(
  src
  "${CMAKE_SOURCE_DIR}/src/core/src"
  "${CMAKE_BINARY_DIR}/src/gui"
  "src/toolwindowmanager/libqtoolwindowmanager"
  ${nclcomposer_core_INCLUDES}
)

set (UI_SOURCES
  ui/PreferencesDialog.ui
  ui/ComposerMainWindow.ui
  ui/PerspectiveManager.ui
  ui/RunGingaConfig.ui
  ui/PluginDetailsDialog.ui
  ui/EnvironmentPreferencesWidget.ui
  ui/WelcomeWidget.ui
  ui/AboutDialog.ui
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
  src/PluginDetailsDialog.cpp
  src/EnvironmentPreferencesWidget.cpp
  src/WelcomeWidget.cpp
  src/AboutDialog.cpp
  src/RunGingaConfig.cpp
  src/GeneralPreferences.cpp
  src/NewProjectWizard.cpp
  src/FileChooser.cpp
  src/ClickableDockWidget.cpp
  src/ComposerSplashScreen.cpp
  src/IPreferencesPage.h

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

target_link_libraries (nclcomposer
  nclcomposer_core
#  sass
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
  Qt5::Network
)

set(GUI_DATAFILES
 data/defaultConnBase.ncl
 data/style.qss
 data/style.ini
 data/default.ini
)

file(
  COPY
    ${GUI_DATAFILES}
  DESTINATION
    ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_DATA_DIR})
  
install(TARGETS nclcomposer DESTINATION bin)
install(
  FILES
    ${GUI_DATAFILES}
  DESTINATION
    ${NCLCOMPOSER_DATA_DIR})

