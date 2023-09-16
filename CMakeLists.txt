cmake_minimum_required(VERSION 3.21)

set(BOARD nucleo_f401re)
find_package(Zephyr)
project(translator)

list(APPEND SOURCES 
    main.cpp
    utils/monitor/monitor.c
)

target_sources(app PRIVATE "${SOURCES}")
