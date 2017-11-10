cmake_minimum_required(VERSION 3.2)
project(nclcomposer)

# get version from git
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(GetGitRevisionDescription)
git_describe(VERSION --tags)
message (STATUS "Configuring NCL Composer ${VERSION}")

if (UNIX)
  add_definitions(-Wextra -Wall)
endif()

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

# nclcomposer - core and gui
add_subdirectory(src/core)
add_subdirectory(src/gui)

# nclcomposer - plugins
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

# target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set (top_srcdir ${CMAKE_SOURCE_DIR})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
                 ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif(DOXYGEN_FOUND)

# Add uninstall target
# Requirements: Copy the uninstall.cmake file to the appropriate
# CMAKE_MODULE_PATH.
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)

# Feature summary
include(FeatureSummary)
feature_summary(WHAT ALL
                INCLUDE_QUIET_PACKAGES
                DESCRIPTION "Enabled Features:"
                VAR enabledFeaturesText)
message(STATUS "${enabledFeaturesText}")

# cpack
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "nclcomposer")
set(CPACK_PACKAGE_VENDOR "TeleMídia Lab/PUC-Rio")

set(CPACK_PACKAGE_VERSION "${VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TeleMidia")
set(CPACK_PACKAGE_CONTACT "Roberto Azevedo <robertogerson@telemidia.puc-rio.br>")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.LGPL")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})

set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")

set(CPACK_DEBIAN_REPLACES "xz")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
  "NCL Composer is an authoring tool for interactive multimedia applications.
   NCL Composer allows you to create interactive digital applications for
   Digital Television using NCL through a simple way. Its extensible
   architecture also allows to anyone developer pluginsthat satisfy each
   specific author profile."
)
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/graphics")

set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libqt5core5a, libqt5gui5, libqt5network5, libqt5xml5, libqt5svg5, \
     libqt5webkit5, libqt5help5")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
# Copy QT's dynamic libs
if(WIN32)
  set(QT_INSTALLED_PATH "C:/Qt/5.8/mingw53_32")
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
          DESTINATION bin/plugins/platforms)
endif()

# This must always be the last!
include(CPack)
