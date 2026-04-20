cmake_minimum_required(VERSION 3.10)
project(sm_online_utils)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ROOT ---
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# --- ANAROOT ---
if(NOT DEFINED ENV{TARTSYS})
    message(FATAL_ERROR "Environment variable TARTSYS is not defined. Please source setup.sh of ANAROOT.")
endif()
set(ANAROOT_INCLUDE_DIR $ENV{TARTSYS}/include)
set(ANAROOT_LIB_DIR $ENV{TARTSYS}/lib)

# include path
include_directories(
    ${ROOT_INCLUDE_DIRS}
    ${ANAROOT_INCLUDE_DIR}
)

# default install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "..." FORCE)
endif()


# sub directories
add_subdirectory(converter)
#add_subdirectory(onlinemonitor)
