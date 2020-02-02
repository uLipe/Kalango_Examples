# Kalango, a always experimental just for fun RTOS:
Simple preemptive-cooperative, realtime, multitask kernel made just for fun and aims to be used
to learn basics and the internals of multitasking programming on microcontrollers, the kernel
is engineered to be simple and scalable allowing others to download, use, learn and scale it
into more professional projects. It is under development status and all updates will
figure here.

## Examples:
Here you will find some examples for popular boards, which I will be adding gradually
the idea is to keep kernel and examples repositories separated to make the integration 
simple, but don't worry, the kernel and other repositories are pinned here using 
git submodules, just clone this repo with option <b>--recursive</b> and all dependencies
should be pulled to local folder. 

## Getting started:
- First get this respository and all the submodules:
 ```
 $ git clone --recursive https://github.com/uLipe/Kalango_Examples
 ```

- Most of examples uses meson build-system, so install it first or move 
the desired example inside your preferred IDE;

- If you want to use meson, after it is installed, just hit commands below 
on one of the samples folder:

```
$ meson  build --cross-file cross-file.txt
$ cd build
$ ninja hex
```
- Inside of the <b>build</b> folder you'll find the .elf and .hex flashable files,
so just burn it into your target board.

## Support:
- If you want some help with this work just contact me: ryukokki.felipe@gmail.com

