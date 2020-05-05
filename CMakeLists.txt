cmake_minimum_required(VERSION 3.7)
project(unidos)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        *.c
        *.cpp
        *.h
        */*.c
        */*.cpp
        */*.h)

string(REGEX REPLACE "CMakeFiles/[^;]+;?" "" SOURCE_FILES "${SOURCE_FILES}")

include_directories(.)
include_directories(ints)
include_directories(util)

include_directories(${LIBUNICORN_INCLUDE_DIR})
link_libraries(${LIBUNICORN_LIBRARY} pthread)

find_package(Unicorn REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(unidos ${SOURCE_FILES})