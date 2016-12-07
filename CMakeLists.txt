cmake_minimum_required(VERSION 3.5)

project(nclcomposer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src/core)
add_subdirectory(src/gui)

# plugins
add_subdirectory(src/plugins/ncl-profile)
add_subdirectory(src/plugins/debug-console)
add_subdirectory(src/plugins/outline-view)
add_subdirectory(src/plugins/properties-view)
add_subdirectory(src/plugins/ncl-layout-view)
add_subdirectory(src/plugins/ncl-structural-view)
add_subdirectory(src/plugins/ncl-rules-view)

# cpack
set(CPACK_PACKAGE_NAME "nclcomposer")
set(CPACK_PACKAGE_VENDOR "TeleMídia Lab/PUC-Rio")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "NCL Composer - Authoring tool for interactive multimedia applications")
set(CPACK_PACKAGE_VERSION "0.3.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TeleMidia")

# This must always be last!
include(CPack)
