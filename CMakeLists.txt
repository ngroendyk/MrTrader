# Written By: Nate Groendyk
# Date: 23 Oct 2018
# Description: Top level CMake file.
# CMake recipe for building this project

cmake_minimum_required (VERSION 3.2)
project (MrTrader)

#Include modules we use
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)


#Force std c++11, and debug flags. Also want to see the build line
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_VERBOSE_MAKEFILE ON)

#Make sure user has boost libs installed
#find_package(Boost COMPONENTS system thread REQUIRED)

# Make sure all the executables end up in ./bin
# as per article: 
#   https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
#   https://cmake.org/cmake/help/v3.0/variable/CMAKE_BINARY_DIR.html
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#Make sure our global header include dir gets included in all our build lines.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


# Lets keep CMakeList files out of 3rd party srcs we dont really control, but just use and built from git src.

#--Gumbo Parser (C - HTML 5 Parser)--#
# Need to try to enforce it to build.
# Check if the lib is there, if not do its build cycle.
# Need to setup a target for its lib, so we can link to it when needed.
#Note the <TARGET> (first parm) needs tothe the directory name of the subtree module/project
ExternalProject_Add(gumbo_parser
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser 
    DOWNLOAD_COMMAND "" 
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser/src
    CONFIGURE_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser/autogen.sh && ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser/configure
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser
    BUILD_COMMAND make
    INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser
    INSTALL_COMMAND make
)

#Let CMake become aware of our third party libgumbo.so file, we will need to link against.
add_library(gumbo SHARED IMPORTED)

#Tell CMake where to locate said libgumbo.so file.
set_property(TARGET gumbo PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/gumbo_parser/.libs/libgumbo.so)

#Tell CMake about the dependancy the external project has to libgumbo
add_dependencies(gumbo gumbo_parser)

# Need to include any of its headers.


##--Third Party WordNet library--##


##-----Now lets build the rest of the app-----##

#Build the src dir.
add_subdirectory(src)





