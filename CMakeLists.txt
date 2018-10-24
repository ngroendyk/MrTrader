# Written By: Nate Groendyk
# Date: 23 Oct 2018
# Description: Top level CMake file.
# CMake recipe for building this project

cmake_minimum_required (VERSION 2.6)
project (MrTrader)



##include the headers and src dirs
#include_directories(src)
#include_directories(include)

#Force std c++11, and debug flags. Also want to see the build line
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_VERBOSE_MAKEFILE ON)

#Make sure user has boost libs installed
#find_package(Boost COMPONENTS system thread REQUIRED)

#Build test curl App
#add_executable(test.exc src/test.cpp src/CurlManager.cpp src/NewsFinder.cpp)

#Link in lib curl
#target_link_libraries(test.exc curl)

# Make sure all the executables end up in ./bin
# as per article: 
#   https://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir
#   https://cmake.org/cmake/help/v3.0/variable/CMAKE_BINARY_DIR.html
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#Make sure our global include dir gets included in all our build lines.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Build the src dir.
add_subdirectory(src)





