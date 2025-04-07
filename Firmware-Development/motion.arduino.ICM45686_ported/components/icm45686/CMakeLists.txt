zephyr_include_directories(.)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_driver_advanced.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_driver.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_edmp.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_selftest.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_transport.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_driver_aux1.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_edmp_compass.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_edmp_wearable.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_i2cm.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inv_imu_transport.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ICM45686.c)





