project(tea5767_i2c_mockup DESCRIPTION "TEA5767 I2C Driver Mockup")

add_library(tea5767_i2c_mockup)

target_sources(tea5767_i2c_mockup PUBLIC tea5767_i2c_mockup.h tea5767_i2c_mockup.c)

target_include_directories(tea5767_i2c_mockup PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

