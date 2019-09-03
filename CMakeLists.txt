cmake_minimum_required(VERSION 3.12)
project(mixedList C)

set(CMAKE_C_STANDARD 99)

add_executable(mixedList main.c list.c Display.c TextObject.c PicObject.c RGB.c)