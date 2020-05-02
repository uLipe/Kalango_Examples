cmake_minimum_required(VERSION 3.5)
project(blink_leds_nrf52 C)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Kalango build)

include_directories(config)
include_directories($ENV{EXAMPLES_PATH}/ext/cmsis)
include_directories($ENV{EXAMPLES_PATH}/ext/nrfx)
include_directories($ENV{EXAMPLES_PATH}/ext/nrfx/mdk)
include_directories($ENV{EXAMPLES_PATH}/ext/segger)
include_directories($ENV{EXAMPLES_PATH}/ext/segger/rtt)
include_directories($ENV{EXAMPLES_PATH}/ext/segger/systemview)


set(SRCS "main.c")
file(GLOB startup "startup_code/*")
list(APPEND ${SRCS} ${startup})

set(LINKER_SCRIPT "ld/nrf52832_xxaa.ld")

add_compile_options(
    -includekalango_config.h
    -DNRF52
    -DNRF52832_XXAA
    -O0 
    -g3    
)

add_executable(${PROJECT_NAME}  ${SRCS})
target_link_libraries(${PROJECT_NAME} -T${LINKER_SCRIPT}  kalango_kernel_lib kalango_external_lib kalango_utils_lib)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)