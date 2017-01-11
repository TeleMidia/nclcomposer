cmake_minimum_required(VERSION 3.2)
project(nclcomposer)
set(VERSION "0.3.1")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (NCLCOMPOSER_BINARY_DIR_BIN bin/)
set (NCLCOMPOSER_BINARY_DIR_LIB lib/${CMAKE_PROJECT_NAME})
set (NCLCOMPOSER_BINARY_DIR_PLUGINS lib/${CMAKE_PROJECT_NAME}/plugins)
set (NCLCOMPOSER_DATA_DIR share/${CMAKE_PROJECT_NAME})

# Looking for Qt >= 5.4
message("-- Looking for Qt5 (>=5.4)")
find_package(Qt5Widgets REQUIRED)

if (Qt5Widgets_FOUND)
  if (Qt5Widgets_VERSION VERSION_LESS 5.4.0)
    message(FATAL_ERROR "Minimum supported Qt5 version is 5.4!")
  else()
    message("-- Looking for Qt5 (>=5.4) -- found!")
  endif()
else()
  message(SEND_ERROR "The Qt5 (>=5.4) library could not be found!")
endif(Qt5Widgets_FOUND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_BINARY_DIR_BIN})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_BINARY_DIR_LIB})

# NCLCOMPOSER - CORE AND GUI
add_subdirectory(src/core)
add_subdirectory(src/gui)

# NCLCOMPOSER - PLUGINS
if(WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_BINARY_DIR_PLUGINS})
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${NCLCOMPOSER_BINARY_DIR_PLUGINS})
endif()

add_subdirectory(src/plugins/ncl-profile)
add_subdirectory(src/plugins/debug-console)
add_subdirectory(src/plugins/outline-view)
add_subdirectory(src/plugins/properties-view)
add_subdirectory(src/plugins/ncl-layout-view)
add_subdirectory(src/plugins/ncl-structural-view)
#add_subdirectory(src/plugins/ncl-textual-view)
add_subdirectory(src/plugins/ncl-rules-view)
add_subdirectory(src/plugins/validator)

if( NOT EXISTS "${CMAKE_SOURCE_DIR}/src/plugins/gingagui-all/.git" )
  execute_process(COMMAND git submodule update --init
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
add_subdirectory(src/plugins/gingagui-all/run-view)

# cpack
set(CPACK_PACKAGE_NAME "nclcomposer")
set(CPACK_PACKAGE_VENDOR "TeleMídia Lab/PUC-Rio")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NCL Composer - Authoring tool for interactive multimedia applications.")

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_VERSION "0.3.1")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TeleMidia")
set(CPACK_PACKAGE_CONTACT "Roberto Azevedo <robertogerson@telemidia.puc-rio.br>")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.LGPL")

# Copy QT's dynamic libs
if(WIN32)
  set(QT_INSTALLED_PATH "C:/Qt/5.5/mingw492_32")
  install(FILES "${QT_INSTALLED_PATH}/bin/Qt5Core.dll"
                "${QT_INSTALLED_PATH}/bin/Qt5Gui.dll"
                "${QT_INSTALLED_PATH}/bin/Qt5Network.dll"
                "${QT_INSTALLED_PATH}/bin/Qt5PrintSupport.dll"
                "${QT_INSTALLED_PATH}/bin/Qt5Widgets.dll"
                "${QT_INSTALLED_PATH}/bin/Qt5Xml.dll"
                "${QT_INSTALLED_PATH}/bin/libgcc_s_dw2-1.dll"
                "${QT_INSTALLED_PATH}/bin/libstdc++-6.dll"
                "${QT_INSTALLED_PATH}/bin/libwinpthread-1.dll"
          DESTINATION bin)

  install(FILES "${QT_INSTALLED_PATH}/plugins/platforms/qwindows.dll"
          DESTINATION plugins/platforms)
endif()

# This must always be last!
include(CPack)
