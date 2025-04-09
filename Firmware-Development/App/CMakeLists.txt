cmake_minimum_required(VERSION 3.20.0)


# Append UBXLIB as a Zephyr module
get_filename_component(UBXLIB_BASE ${CMAKE_CURRENT_LIST_DIR}/components/GNSS/ubxlib  ABSOLUTE)


# Add ubxlib as a module
list(APPEND ZEPHYR_EXTRA_MODULES ${UBXLIB_BASE})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(App)

target_sources(app PRIVATE src/main.cpp)

# External Modules
add_subdirectory(components/BLE)
add_subdirectory(components/IMU)
add_subdirectory(components/GNSS)
add_subdirectory(components/Magnetometer)