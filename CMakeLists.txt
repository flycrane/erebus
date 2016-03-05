cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(erebus CXX)

include_directories(src)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror")

add_subdirectory(src)

add_executable(erebus ${SOURCE})
