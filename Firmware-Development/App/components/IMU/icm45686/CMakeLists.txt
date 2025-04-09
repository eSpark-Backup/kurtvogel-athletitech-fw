find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})


zephyr_include_directories(.)

target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ICM45686.cpp)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_driver_advanced.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_driver.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_transport.c)


