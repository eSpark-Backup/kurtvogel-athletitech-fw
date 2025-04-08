cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(arduino_icm45686_ported)

target_sources(app PRIVATE src/main.cpp)

add_subdirectory(components/icm45686)

