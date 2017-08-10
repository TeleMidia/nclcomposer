cmake_minimum_required (VERSION 3.2)
project(nclcomposer_gui)

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
  PluginDetailsDialog.ui
  EnvironmentPreferencesWidget.ui
  WelcomeWidget.ui
  AboutDialog.ui
  ShortcutsDialog.ui
  GeneralPreferences.ui
  NewProjectWizard.ui
  FileChooser.ui
  QSimpleUpdater/src/Downloader.ui
  PreferencesEditor.ui
)

QT5_WRAP_UI(UI_GENERATED_HEADERS
  ${UI_SOURCES}
)
# Qt5 Translations
find_package (Qt5LinguistTools)
set_directory_properties (PROPERTIES CLEAN_NO_CUSTOM 1)
qt5_create_translation (QM_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}

  translations/nclcomposer_en.ts
  translations/nclcomposer_es_ES.ts
  translations/nclcomposer_pt_BR.ts
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

  PreferencesEditor.cpp

  ${UI_GENERATED_HEADERS}
  gui.qrc
  ../plugins/common-resources/images.qrc
  ../plugins/common-resources/language.qrc
  QSimpleUpdater/etc/resources/qsimpleupdater.qrc
  
  nclcomposer.rc
  ${QM_FILES}
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

  ${QM_FILES}
)

# Copy datafiles to build dir
add_custom_target(copy_datafiles ALL)
add_dependencies(copy_datafiles nclcomposer)
foreach(file_i ${GUI_DATAFILES})
  get_filename_component(filename ${file_i} NAME)

  # if it is not absoute transform it to absolute path
  set(INPUT_FILE ${file_i})
  if(NOT IS_ABSOLUTE ${INPUT_FILE})
    set (INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}")
  endif()

  add_custom_command(TARGET copy_datafiles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${INPUT_FILE}
      ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_DATA_DIR}/${filename}
    DEPENDS ${file_i}
    COMMENT "Copying ${file_i}")
endforeach(file_i)

install(TARGETS nclcomposer DESTINATION bin)
install(FILES ${GUI_DATAFILES} DESTINATION ${NCLCOMPOSER_DATA_DIR})

install(FILES images/16x16/nclcomposer.png DESTINATION share/icons/hicolor/16x16/apps)
install(FILES images/22x22/nclcomposer.png DESTINATION share/icons/hicolor/22x22/apps)
install(FILES images/24x24/nclcomposer.png DESTINATION share/icons/hicolor/24x24/apps)
install(FILES images/32x32/nclcomposer.png DESTINATION share/icons/hicolor/32x32/apps)
install(FILES images/48x48/nclcomposer.png DESTINATION share/icons/hicolor/48x48/apps)
install(FILES images/64x64/nclcomposer.png DESTINATION share/icons/hicolor/64x64/apps)
install(FILES images/128x128/nclcomposer.png DESTINATION share/icons/hicolor/128x128/apps)
install(FILES images/256x256/nclcomposer.png DESTINATION share/icons/hicolor/256x256/apps)
install(FILES images/nclcomposer.svg DESTINATION share/icons/hicolor/scalable/apps)
install(FILES data/nclcomposer.desktop DESTINATION share/applications)

