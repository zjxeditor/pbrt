cmake_minimum_required(VERSION 3.7)
project(pbrt)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(pbrt ${SOURCE_FILES})