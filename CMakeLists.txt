cmake_minimum_required(VERSION 3.2)
project(nclcomposer)

# get version from git
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(GetGitRevisionDescription)
git_describe(VERSION --tags)
message (STATUS "Configuring NCL Composer ${VERSION}")

#parse the version information into pieces.
string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
# end - sget version from git

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NCLCOMPOSER_BINARY_DIR_BIN bin/)
set(NCLCOMPOSER_BINARY_DIR_LIB lib/${CMAKE_PROJECT_NAME})
set(NCLCOMPOSER_BINARY_DIR_PLUGINS lib/${CMAKE_PROJECT_NAME}/plugins)
set(NCLCOMPOSER_DATA_DIR share/${CMAKE_PROJECT_NAME})

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
add_subdirectory(src/plugins/ncl-textual-view)
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

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_PACKAGE_VERSION "${VERSION}")
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
          DESTINATION
                bin)

  install(FILES "${QT_INSTALLED_PATH}/plugins/platforms/qwindows.dll"
          DESTINATION plugins/platforms)
endif()

# This must always be last!
include(CPack)
