#CMake recipe for building this project

cmake_minimum_required (VERSION 2.6)
project (MrTrader)

#include the headers and src dirs
include_directories(src)
include_directories(include)

#Force std c++11, and debug flags. Also want to see the build line
set(CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
set(CMAKE_VERBOSE_MAKEFILE ON)

#Make sure user has boost libs installed
#find_package(Boost COMPONENTS system thread REQUIRED)

#Build test curl App
add_executable(test.exc src/test.cpp src/CurlManager.cpp src/NewsFinder.cpp)

#Link in lib curl
target_link_libraries(test.exc curl)


