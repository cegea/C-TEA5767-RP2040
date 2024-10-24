if(BUILD_TARGET EQUAL 0)
    set(This "radio")
    add_executable(${This}
        radio.c
        # put additional source files here
    )

    target_include_directories(
        ${This} PUBLIC "../tea5767_i2c"
        ${PROJECT_BINARY_DIR}
        # put your include directories here
    )

    # pull in common dependencies
    target_link_libraries(
        ${This}
        pico_stdlib
        tea5767_i2c_driver 
        pico_stdlib
        hardware_gpio
    )

    # enable usb output, disable uart output
    pico_enable_stdio_usb(${This} 0)
    pico_enable_stdio_uart(${This} 1)

    # create map/bin/hex/uf2 file etc.
    pico_add_extra_outputs(${This})
endif()

if(BUILD_TARGET EQUAL 1)
    set(This "radio")
    add_executable(${This}
        radio.c
        # put your source files here
    )

    target_include_directories(
        ${This} PUBLIC "../tea5767_i2c"
        ${PROJECT_BINARY_DIR}
        # put your include directories here
    )

    # pull in common dependencies
    target_link_libraries(
        ${This}
        pico_stdlib
        tea5767_i2c_driver 
        pico_stdlib
        hardware_gpio
    )
endif()

if(BUILD_TARGET EQUAL 2)
    set(This "libApp")

    set(Headers
        # put the header files you need for testing here
    )
    set(Sources
        # put the source files of your productive code here
    )

    add_library(${This} STATIC ${Sources}) # this is to build a library for testing
    target_include_directories(${This} PUBLIC "../include")

    target_link_libraries(
        ${This} PUBLIC
        # put additional libraries here
    )

endif()