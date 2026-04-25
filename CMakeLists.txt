cmake_minimum_required(VERSION 3.10)
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

add_subdirectory(converter)
add_subdirectory(dceffplot)
add_subdirectory(onlinemonitor)
#add_subdirectory(filter)
