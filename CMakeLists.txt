cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

find_package(IRODS REQUIRED CONFIG)

set(IRODS_PLUGIN_REVISION "0")
set(IRODS_PLUGIN_VERSION "${IRODS_VERSION}.${IRODS_PLUGIN_REVISION}")

set(IRODS_PACKAGE_REVISION "1")

include(RequireOutOfSourceBuild)
include(IrodsCXXCompiler)

set(CMAKE_CXX_STANDARD ${IRODS_CXX_STANDARD})
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# export-dynamic so stacktrace entries from executables have function names.
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-rdynamic -Wl,--export-dynamic -Wl,--enable-new-dtags -Wl,--as-needed -Wl,-z,defs")
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE_INIT "-Wl,--gc-sections -Wl,-z,combreloc")

project(
  irods_get_rescinfo_for_get_cpp_api_plugin
  VERSION "${IRODS_VERSION}"
  LANGUAGES CXX)

set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_SKIP_INSTALL_RPATH OFF)
set(CMAKE_SKIP_RPATH OFF)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)

include(${IRODS_TARGETS_PATH})
include(GNUInstallDirs)
include(UseLibCXX)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build {Debug, Release}." FORCE)
  message(STATUS "Setting unspecified CMAKE_BUILD_TYPE to '${CMAKE_BUILD_TYPE}'. This is the correct setting for normal builds.")
endif()

find_package(nlohmann_json "3.6.1" REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
endif()

add_compile_options(-Wall -Wextra -Werror -Wconversion -Wshadow -Wpedantic)

set(IRODS_MODULE_NAME_PREFIX "irods_cpp_get_rescinfo_for_get_api_plugin")

foreach (IRODS_MODULE_VARIANT IN ITEMS client server)
  set(IRODS_MODULE_NAME ${IRODS_MODULE_NAME_PREFIX}_${IRODS_MODULE_VARIANT})

  add_library(
    ${IRODS_MODULE_NAME}
    MODULE
    ${CMAKE_SOURCE_DIR}/src/${IRODS_MODULE_VARIANT}.cpp
    ${CMAKE_SOURCE_DIR}/src/plugin_factory.cpp)
  
  target_compile_definitions(
    ${IRODS_MODULE_NAME}
    PRIVATE
    ${IRODS_COMPILE_DEFINITIONS}
    ${IRODS_COMPILE_DEFINITIONS_PRIVATE})
  
  target_include_directories(
    ${IRODS_MODULE_NAME}
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    ${IRODS_INCLUDE_DIRS}
    ${IRODS_EXTERNALS_FULLPATH_BOOST}/include
    ${IRODS_EXTERNALS_FULLPATH_FMT}/include
    ${IRODS_EXTERNALS_FULLPATH_NANODBC}/include
    ${IRODS_EXTERNALS_FULLPATH_SPDLOG}/include)
  
  target_link_libraries(
    ${IRODS_MODULE_NAME}
    PRIVATE
    irods_plugin_dependencies
    irods_common
    irods_${IRODS_MODULE_VARIANT}
    ${IRODS_EXTERNALS_FULLPATH_FMT}/lib/libfmt.so)

  install(
    TARGETS ${IRODS_MODULE_NAME}
    LIBRARY DESTINATION "${IRODS_PLUGINS_DIRECTORY}/api")
endforeach()

target_compile_definitions(
  ${IRODS_MODULE_NAME_PREFIX}_server
  PRIVATE
  RODS_SERVER
  ENABLE_RE
  IRODS_ENABLE_SYSLOG)

install(
  FILES
  "${CMAKE_SOURCE_DIR}/include/irods/plugins/api/get_rescinfo_for_get_common.h"
  #"${CMAKE_SOURCE_DIR}/include/irods/plugins/api/rc_get_rescinfo_for_get.h"
  #"${CMAKE_SOURCE_DIR}/include/irods/plugins/api/rs_get_rescinfo_for_get.hpp"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/irods/plugins/api")

# TODO Figure out how to ship the convenience headers/implementations for invoking the API plugin.
# We don't have anything in place for clients to automatically load convenience logic. Maybe we
# just provide examples for how to invoke the plugins from a client and a server.
#set(IRODS_PLUGIN_INVOCATION_FUNCTIONS irods_get_rescinfo_for_get_invocation_functions)
#
#add_library(
#  ${IRODS_PLUGIN_INVOCATION_FUNCTIONS}
#  SHARED
#  ${CMAKE_SOURCE_DIR}/src/rc_get_rescinfo_for_get.cpp
#  ${CMAKE_SOURCE_DIR}/src/rs_get_rescinfo_for_get.cpp)
#
#target_include_directories(
#  ${IRODS_MODULE_NAME}
#  PRIVATE
#  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#  ${IRODS_INCLUDE_DIRS}
#  #${IRODS_EXTERNALS_FULLPATH_BOOST}/include
#  #${IRODS_EXTERNALS_FULLPATH_FMT}/include
#  #${IRODS_EXTERNALS_FULLPATH_NANODBC}/include
#  #${IRODS_EXTERNALS_FULLPATH_SPDLOG}/include)
#  )
#
#target_link_libraries(
#  ${IRODS_PLUGIN_INVOCATION_FUNCTIONS}
#  PRIVATE
#  irods_server)

include(IrodsCPackCommon)
include(IrodsCPackPlatform)

# TODO Use the same naming convention for all project templates.
set(IRODS_PLUGIN_PACKAGE_NAME irods-api-plugin-get-rescinfo-for-get)

set(CPACK_PACKAGE_VERSION ${IRODS_PLUGIN_VERSION})
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENTS_GROUPING IGNORE)
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The integrated Rule-Oriented Data System") # Defined by IrodsCPackCommon

set(CPACK_DEB_COMPONENT_INSTALL OFF)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)
#set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/packaging/postinst;")
#set(CPACK_DEBIAN_PACKAGE_BREAKS ${IRODS_PLUGIN_PACKAGE_NAME})
#set(CPACK_DEBIAN_PACKAGE_REPLACES ${IRODS_PLUGIN_PACKAGE_NAME})
set(CPACK_DEBIAN_PACKAGE_RELEASE "${IRODS_PACKAGE_REVISION}")
if (IRODS_LINUX_DISTRIBUTION_VERSION_CODENAME)
  set(CPACK_DEBIAN_PACKAGE_RELEASE "${CPACK_DEBIAN_PACKAGE_RELEASE}~${IRODS_LINUX_DISTRIBUTION_VERSION_CODENAME}")
endif()
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

set(CPACK_RPM_COMPONENT_INSTALL OFF)
set(CPACK_RPM_PACKAGE_LICENSE "BSD-3-Clause")
set(CPACK_RPM_PACKAGE_AUTOREQ 0)
set(CPACK_RPM_PACKAGE_AUTOPROV 0)
#set(CPACK_RPM_PACKAGE_OBSOLETES ${IRODS_PLUGIN_PACKAGE_NAME})
#set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/packaging/postinst")
set(CPACK_RPM_PACKAGE_RELEASE "${IRODS_PACKAGE_REVISION}")
set(CPACK_RPM_FILE_NAME RPM-DEFAULT)

set(CPACK_ARCHIVE_COMPONENT_INSTALL OFF)

set(CPACK_DEBIAN_PACKAGE_NAME ${IRODS_PLUGIN_PACKAGE_NAME})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "${IRODS_PACKAGE_DEPENDENCIES_STRING}, irods-runtime (= ${IRODS_VERSION})")

set(CPACK_RPM_PACKAGE_NAME ${IRODS_PLUGIN_PACKAGE_NAME})
set(CPACK_RPM_PACKAGE_REQUIRES "${IRODS_PACKAGE_DEPENDENCIES_STRING}, irods-runtime = ${IRODS_VERSION}")

include(CPack)
