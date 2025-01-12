project(tea5767_i2c_test DESCRIPTION "Testing framework")

# find_library(tea5767_i2c_driver tea5767_i2c_driver PATHS "${CMAKE_SOURCE_DIR}/../build/src" NO_DEFAULT_PATH)

# if(NOT tea5767_i2c_driver)
#     message(FATAL_ERROR "No se encontró la biblioteca tea5767_i2c_driver en ../build")
# endif()

add_subdirectory(tea5767_i2c_mockup)

add_executable(tea5767_i2c_test tea5767_i2c_test.c)
add_executable(tea5767_i2c_test_mockup tea5767_i2c_test_mockup.c)

target_link_libraries(tea5767_i2c_test 
tea5767_i2c_mockup
tea5767_i2c_driver 
unity) 

target_link_libraries(tea5767_i2c_test_mockup 
tea5767_i2c_mockup 
unity) 

add_test(
    NAME tea5767_i2c_test 
    COMMAND $<TARGET_FILE:tea5767_i2c_test>
)

add_test(
    NAME tea5767_i2c_test_mockup 
    COMMAND $<TARGET_FILE:tea5767_i2c_test_mockup>
)

