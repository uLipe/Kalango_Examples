# Kalango, a always experimental just for fun RTOS:
Simple preemptive-cooperative, realtime, multitask kernel made just for fun and aims to be used
to learn basics and the internals of multitasking programming on microcontrollers, the kernel
is engineered to be simple and scalable allowing others to download, use, learn and scale it
into more professional projects. It is under development status and all updates will
figure here.

# Examples:
Here you will find some examples for popular boards, which I will be adding gradually
the idea is to keep kernel and examples repositories separated to make the integration 
simple, but don't worry, the kernel and other repositories are pinned here using 
git submodules, just clone this repo with option <b>--recursive</b> and all dependencies
should be pulled to local folder. 

# Get the Code!
 ```
 $ git clone --recursive https://github.com/uLipe/Kalango_Examples
 ```

# Build a sample!
Building a sample is simple, just navigate to one of sample directories, and 
using the CMake (you need to install it) type:

```
$ export EXAMPLES_PATH=/path/to/Kalango_Examples/folder
$ cd blink-leds-nrf52-dk
$ mkdir build && cd build
$ cmake -GNinja .. -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-none-eabi-m4-float-toolchain.cmake -DKALANGO_CONFIG_FILE_PATH=../config_includes
```
Inside of the build directory you should now able to build the firmware:

```
$ ninja
```
The <b>build</b> folder now has the .elf file which can be converted to a binary file(hex or bin), or flashed to the target using
your favorite debug probe.

# Support:
- If you want some help with this work just contact me: ryukokki.felipe@gmail.com

