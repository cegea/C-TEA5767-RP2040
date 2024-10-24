project(tea5767_i2c_driver DESCRIPTION "TEA5767 I2C Driver")

add_library(tea5767_i2c_driver)

target_sources(tea5767_i2c_driver PUBLIC tea5767_i2c.h tea5767_i2c.c)
target_link_libraries(tea5767_i2c_driver pico_stdlib hardware_i2c hardware_gpio)

add_library(tea5767_i2c_driver::tea5767_i2c_driver ALIAS tea5767_i2c_driver)