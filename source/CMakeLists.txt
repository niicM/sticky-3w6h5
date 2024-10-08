add_executable(source) 

target_sources(source PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
    ${CMAKE_CURRENT_LIST_DIR}/usb_hid_out_fn.c
    ${CMAKE_CURRENT_LIST_DIR}/input.c
    ${CMAKE_CURRENT_LIST_DIR}/input_right.c
    ${CMAKE_CURRENT_LIST_DIR}/input_left.c
    ${CMAKE_CURRENT_LIST_DIR}/print_buff.c
    ${CMAKE_CURRENT_LIST_DIR}/layouts.c

    ${CMAKE_CURRENT_LIST_DIR}/e_effects.c
    ${CMAKE_CURRENT_LIST_DIR}/e_key_mod_effect.c
    ${CMAKE_CURRENT_LIST_DIR}/e_press_to_effect.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(source PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(source PUBLIC pico_stdlib pico_unique_id 
    tinyusb_device tinyusb_board hardware_i2c pico_multicore
)

pico_add_extra_outputs(source)
