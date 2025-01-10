# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")


cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
# BUILD_Target == 0 ==> pico
# BUILD_Target == 2 ==> test
set(BUILD_TARGET 0)
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICO_SDK_FETCH_FROM_GIT_TAG 1.5.1) 

if(BUILD_TARGET EQUAL 0)
    include(pico_sdk_import.cmake)
    add_definitions(-DPICO)
endif()

project(TEA5767_I2C_DRIVER_EXAMPLE C CXX ASM)

if(BUILD_TARGET EQUAL 0)
    pico_sdk_init()
    add_subdirectory(tea5767_i2c) # Will add the library tea5767_i2c
    add_subdirectory(radio) # Will add the library tea5767_i2c
endif()

if(BUILD_TARGET EQUAL 2)
    add_subdirectory(test)
    add_subdirectory(googletest)
    add_definitions(-DTEST)
    add_subdirectory(src) # Will add the library tea5767_i2c
    add_subdirectory(radio)
endif()

# add_executable(${PROJECT_NAME} example/radio.c)
# add_subdirectory(test) # Will add the library tea5767_i2cz

# target_link_libraries(${PROJECT_NAME} tea5767_i2c_driver pico_stdlib hardware_gpio)

# target_link_libraries(${PROJECT_NAME} tea5767_i2c)

# create map/bin/hex/uf2 file in addition to ELF.
# pico_add_extra_outputs(${PROJECT_NAME})

