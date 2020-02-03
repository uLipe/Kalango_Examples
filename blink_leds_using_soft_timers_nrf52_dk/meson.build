project('nrf52_blink', 'c')

compiler_obj = meson.get_compiler('c')
size = find_program('arm-none-eabi-size')
objcopy = find_program('arm-none-eabi-objcopy')

cputype = 'cortex-m4'
cpudefine = 'nrf52'

kalango_path = '../Kalango'
kernel_sources = []
arch_sources = []
target_includes = []
log_sources = []
lib_sources = []

target_includes += [
   include_directories(kalango_path + '/include'),
   include_directories(kalango_path + '/lib'),
   include_directories('../ext/cmsis'),
   include_directories('../ext/nrfx'),
   include_directories('../ext/nrfx/mdk'),
   include_directories('../ext/printf'), 
   include_directories('../ext/segger'),
   include_directories('../ext/segger/rtt'),
   include_directories('../ext/segger/systemview'),
   include_directories('config')
]

log_sources += files(
   '../ext/segger/rtt/SEGGER_RTT.c',
   '../ext/segger/rtt/SEGGER_RTT.c',
   '../ext/segger/systemview/SEGGER_SYSVIEW.c',
   '../ext/segger/systemview/SEGGER_SYSVIEW.c',
   '../ext/printf/printf.c',
)

lib_sources += files(
   kalango_path + '/lib/print_out.c',
   kalango_path + '/lib/tlsf.c',
)

kernel_sources += files(
   kalango_path + '/src/clock.c',
   kalango_path + '/src/core.c',
   kalango_path + '/src/mutex.c',
   kalango_path + '/src/object_pool.c',
   kalango_path + '/src/queue.c',
   kalango_path + '/src/sched_fifo.c',
   kalango_path + '/src/sched_round_robin.c',
   kalango_path + '/src/semaphore.c',
   kalango_path + '/src/task.c',
   kalango_path + '/src/timer.c',
)

arch_sources += files(
   kalango_path + '/arch/arm_v7m/arch_armv7m.c',
   kalango_path + '/arch/arm_v7m/arch_armv7m_asm.S',
)

kalango_srcs = kernel_sources + arch_sources + lib_sources + log_sources

nrf52_sources =  []
app_sources =  []

nrf52_sources += files(
   'startup_code/gcc_startup_nrf52833.S',
   'startup_code/system_nrf52833.c',
)

app_sources += files(
   'main.c',
)

srcs = kalango_srcs + app_sources + nrf52_sources

cargs =  [
   '-includekalango_config.h',
   '-DNRF52',
   '-DNRF52832_XXAA',
   '-mcpu=cortex-m4',
   '-mthumb',
   '-mabi=aapcs',
   '-Wall', 
   '-Werror',
   '-O0', 
   '-g3',
   '-mfloat-abi=hard',
   '-mfpu=fpv4-sp-d16',
   '-ffunction-sections',
   '-fdata-sections',
   '-fno-strict-aliasing',
   '-fno-builtin',
   '--short-enums',
]

linkargs =  [
   '-mthumb',
   '-mabi=aapcs',
   '-T' + meson.current_source_dir() + '/nrf52832_xxaa.ld',
   '-mcpu=cortex-m4',
   '-mfloat-abi=hard',
   '-mfpu=fpv4-sp-d16',
   '-Wl,--gc-sections',
   '--specs=nano.specs',
   '-lc',
   '-lnosys',
   '-lm',
]

artifacts = ['blink', srcs, cargs, linkargs]

exe = executable(artifacts[0], artifacts[1],
                 c_args: artifacts[2],
                 link_args: artifacts[3],
                 include_directories : target_includes,
                 build_by_default: true)

run_target('hex', command: [objcopy, ['-Obinary', exe.full_path(),
           exe.full_path() + '.bin']], depends: exe)

run_target('size', command: [size, exe.full_path(), '-B'], depends: exe)

if meson.is_cross_build()
   message('cross compiling for ' + cputype)
   message('''
              ninja           - generates elf file.
              ninja hex       - generates hex file.
              ninja size      - gives the summary of hex file size.
              (C) uLipe 2019''')
endif

