cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        bf66935e07825318ae519675d73d0f3e313b3ec6 
)
FetchContent_MakeAvailable(googletest)

project(tea5767_i2c_driver_test DESCRIPTION "Testing framework")

find_library(tea5767_i2c_driver tea5767_i2c_driver PATHS "${CMAKE_SOURCE_DIR}/../build/src" NO_DEFAULT_PATH)

if(NOT tea5767_i2c_driver)
    message(FATAL_ERROR "No se encontró la biblioteca tea5767_i2c_driver en ../build")
endif()

add_executable(tea5767_i2c_driver_test tea5767_i2c_test.cpp)

target_link_libraries(tea5767_i2c_driver_test gtest_main gmock_main tea5767_i2c_driver) 

include(GoogleTest)
gtest_discover_tests(tea5767_i2c_driver_test)
