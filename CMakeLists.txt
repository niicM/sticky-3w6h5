cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
# include(pico_extras_import_optional.cmake)

project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)

add_subdirectory(source)
add_subdirectory(dev_hid_composite)