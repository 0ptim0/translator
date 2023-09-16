cmake_minimum_required(VERSION 3.21)

set(BOARD nucleo_f401re)
find_package(Zephyr)
project(translator)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")

target_sources(app PRIVATE main.cpp)
