cmake_minimum_required(VERSION 3.13.1)


target_compile_definitions(app PRIVATE U_CFG_APP_FILTER=exampleGnss)
target_compile_definitions(app PRIVATE U_CFG_TEST_GNSS_MODULE_TYPE=U_GNSS_MODULE_TYPE_M9)
target_compile_definitions(app PRIVATE U_CFG_APP_GNSS_I2C)
target_compile_definitions(app PRIVATE U_CFG_APP_PIN_GNSS_xxx)



zephyr_include_directories(.)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Gnss.cpp)


