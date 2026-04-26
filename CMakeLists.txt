cmake_minimum_required(VERSION 3.10)

set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")

option(BUILD_SMCONVERTER   "Build smconverter"   ON)
option(BUILD_DCEFFPLOT     "Build dceffplot"     ON)
option(BUILD_ONLINEMONITOR "Build onlinemonitor" ON)

project(sm_online_utils)

# default install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "install prefix" FORCE)
endif()

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

if(NOT DEFINED ENV{TARTSYS})
    message(FATAL_ERROR "[ERROR] TARTSYS is not defined")
endif()

set(ANAROOT_INCLUDE_DIR $ENV{TARTSYS}/include)
set(ANAROOT_LIB_DIR     $ENV{TARTSYS}/lib)

include_directories(
    ${ROOT_INCLUDE_DIRS}
    ${ANAROOT_INCLUDE_DIR}
)

install(DIRECTORY macros/ DESTINATION macros/)

if(BUILD_SMCONVERTER)
  add_subdirectory(smconverter)
endif()

if(BUILD_DCEFFPLOT)
    add_subdirectory(dceffplot)
endif()

if(BUILD_ONLINEMONITOR)
    add_subdirectory(onlinemonitor)
endif()

#add_subdirectory(filter)
