
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_LINKER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mabi=aapcs -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4 -mabi=aapcs -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4  -mabi=aapcs  -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections --specs=nano.specs -lc -lm -lnosys")