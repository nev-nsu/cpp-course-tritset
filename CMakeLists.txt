cmake_minimum_required(VERSION 3.6)
project(tritset)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Tritset.cpp Tritset.h Trit.cpp Trit.h)
add_executable(tritset ${SOURCE_FILES})

add_subdirectory(testing)