cmake_minimum_required(VERSION 3.5)
project(button-isr C ASM)

add_subdirectory(KalangoRTOS)

set(SRCS "main.c"
         "system_nrf52833.c"
         "gcc_startup_nrf52833.S")
  
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/ld/nrf52832_xxaa.ld")

add_executable(${PROJECT_NAME} ${SRCS})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -g3 -DNRF52 -DNRF52832_XXAA -O0 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums)

target_link_libraries(${PROJECT_NAME} KalangoRTOS -T${LINKER_SCRIPT})

target_include_directories(${PROJECT_NAME} PUBLIC config_includes)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/cmsis)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/nrfx)
target_include_directories(${PROJECT_NAME} PUBLIC $ENV{EXAMPLES_PATH}/ext/nrfx/mdk)