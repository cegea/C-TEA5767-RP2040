cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS ON)
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICO_SDK_FETCH_FROM_GIT_TAG 1.5.1) 

include(pico_sdk_import.cmake)
include_directories(include src)

project(TEA5767_I2C_DRIVER_EXAMPLE C CXX ASM)
pico_sdk_init()

add_executable(${PROJECT_NAME} example/radio.c)
add_subdirectory(src) # Will add the library tea5767_i2c
add_subdirectory(test) # Will add the library tea5767_i2c

target_link_libraries(${PROJECT_NAME} tea5767_i2c_driver pico_stdlib hardware_gpio)

# target_link_libraries(${PROJECT_NAME} tea5767_i2c)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

