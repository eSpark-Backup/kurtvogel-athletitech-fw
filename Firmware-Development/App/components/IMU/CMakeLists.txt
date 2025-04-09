zephyr_include_directories(.)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/icm45686)

target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/IMU.cpp)

