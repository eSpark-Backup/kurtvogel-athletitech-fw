cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(App)

target_sources(app PRIVATE src/main.cpp)

# External Modules
add_subdirectory(components/BLE)
add_subdirectory(components/icm45686)
add_subdirectory(components/neoM9)
add_subdirectory(components/tlv493)
