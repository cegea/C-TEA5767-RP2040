project(tea5767_i2c_driver DESCRIPTION "TEA5767 I2C Driver")

add_library(tea5767_i2c_driver)

target_sources(tea5767_i2c_driver PUBLIC tea5767_i2c.h tea5767_i2c.c)
if(BUILD_TARGET EQUAL 0)
target_link_libraries(tea5767_i2c_driver pico_stdlib hardware_i2c hardware_gpio m)
endif()

if(BUILD_TARGET EQUAL 2)
target_link_libraries(tea5767_i2c_driver tea5767_i2c_mockup m)
target_include_directories(tea5767_i2c_driver PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)
endif()

#add_library(tea5767_i2c_driver::tea5767_i2c_driver ALIAS tea5767_i2c_driver)