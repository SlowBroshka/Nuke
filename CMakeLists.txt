cmake_minimum_required(VERSION 3.7)
project(untitled)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c)

add_executable(untitled ${SOURCE_FILES})
find_library(M_LIB m)
target_link_libraries(untitled ${M_LIB})