
zephyr_include_directories(.)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tlv493_reg.c)
target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tlv493.c)

