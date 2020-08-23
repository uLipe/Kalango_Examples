cmake_minimum_required(VERSION 3.5)
project(kalango-system-test C ASM)

add_subdirectory(KalangoRTOS)

set(SRCS    "main.c"
            "system_test.c"
            "system_nrf52833.c"
            "gcc_startup_nrf52833.S"
            "systemview/SEGGER_SYSVIEW.c"
            "rtt/SEGGER_RTT.c"
            "rtt/SEGGER_RTT_printf.c"
            "printf/printf.c")  

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ld/nrf52832_xxaa.ld")

add_executable(${PROJECT_NAME} ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -g3 -DNRF52 -DNRF52832_XXAA -O0 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums)

target_link_libraries(${PROJECT_NAME} KalangoRTOS -T${LINKER_SCRIPT})

target_include_directories(${PROJECT_NAME} PUBLIC config_includes)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/cmsis)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/nrfx)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/nrfx/mdk)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/segger)
target_include_directories(${PROJECT_NAME} PUBLIC rtt)
target_include_directories(${PROJECT_NAME} PUBLIC sysview)
target_include_directories(${PROJECT_NAME} PUBLIC printf)



