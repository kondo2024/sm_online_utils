cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(sm_online_utils)

### default install options ----------------------

option(use_pfad "use PFAD" OFF)
option(use_fsdbsd "use FSD/BSD" OFF)

### for ROOT ----------------------
if(DEFINED ENV{ROOTSYS})
  message(STATUS "Found ROOT, ROOTSYS = " $ENV{ROOTSYS})
else()
  message(FATAL_ERROR "\$ROOTSYS is not defined. please define the environment for ROOT.")
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

set(CMAKE_CXX_FLAGS "-fPIC -Wall -Wextra -O3 -mtune=native -march=native -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -O3 -mtune=native -march=native")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_INCLUDE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/include")

set(ROOTSYS $ENV{ROOTSYS})
set(PPWWDD ${CMAKE_SOURCE_DIR})

### for ANAROOT ---------------------------
if(DEFINED ENV{TARTSYS})
  message(STATUS "Found ANAROOT, TARTSYS = " $ENV{TARTSYS})
else()
  message(FATAL_ERROR "\$TARTSYS is not defined. please define the environment for ANAROOT.")
endif()

set(ANAROOT_LIBRARIES)
foreach(_cpt anaroot XMLParser)
  find_library(ANAROOT_${_cpt}_LIBRARY ${_cpt} $ENV{TARTSYS}/lib)
  if(ANAROOT_${_cpt}_LIBRARY)
    list(APPEND ANAROOT_LIBRARIES ${ANAROOT_${_cpt}_LIBRARY})
  endif()
endforeach()

include_directories($ENV{TARTSYS}/include)

###  ---------------------------

add_subdirectory(smconverter)
#add_subdirectory(smfilter)
