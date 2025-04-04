# zephyr_include_directories(TLV493D-A1B6-3DMagnetic-Sensor)

# target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tlv493_reg.c
#                             ${CMAKE_CURRENT_SOURCE_DIR}/tlv493.c
#                            )
# Link Zephyr to make sure Zephyr headers are available

zephyr_include_directories(.)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Magnetometer.cpp)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tlv493.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tlv493_reg.c)

