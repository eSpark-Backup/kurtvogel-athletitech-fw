# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

get_filename_component(UBXLIB_BASE ../.. ABSOLUTE)

# Add ubxlib as a module
list(APPEND ZEPHYR_EXTRA_MODULES ${UBXLIB_BASE})

find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})
project(zephyr_module_test)


target_compile_definitions(app PRIVATE U_CFG_APP_FILTER=exampleGnss)
target_compile_definitions(app PRIVATE U_CFG_TEST_GNSS_MODULE_TYPE=U_GNSS_MODULE_TYPE_M9)
target_compile_definitions(app PRIVATE U_CFG_APP_GNSS_I2C)
target_compile_definitions(app PRIVATE U_CFG_APP_PIN_GNSS_xxx)



# test application
target_sources(app PRIVATE main.c)