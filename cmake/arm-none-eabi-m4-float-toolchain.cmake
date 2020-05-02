
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mabi=aapcs -mthumb -Wall -Werror -g3 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin --short-enums")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4  -mabi=aapcs  -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections --specs=nano.specs -lc  -lnosys")
set(CMAKE_C_COMPILER_FORCED TRUE)