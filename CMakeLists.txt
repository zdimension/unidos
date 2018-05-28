cmake_minimum_required(VERSION 3.7)
project(unidos)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_STANDARD 99)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SOURCE_FILES
        ints/int10.c
        ints/int10.h
        ints/int15.c
        ints/int15.h
        ints/int20.c
        ints/int20.h
        ints/int21.c
        ints/int21.h
        ints/ints.h
        util/dospath.c
        util/dospath.h
        util/uc.c
        util/uc.h
        global.c
        global.h
        main.c)

include_directories(.)
include_directories(ints)
include_directories(util)

include_directories(${LIBUNICORN_INCLUDE_DIR})
link_libraries(${LIBUNICORN_LIBRARY} pthread)

find_package(Unicorn REQUIRED)

add_executable(unidos ${SOURCE_FILES})