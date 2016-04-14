cmake_minimum_required(VERSION 3.3)
project(micropython)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES 3ds/main.c)

include_directories(/opt/devkitpro/devkitARM/arm-none-eabi/include)
include_directories(/opt/devkitpro/libctru/include)
include_directories(/opt/devkitpro/portlibs/armv6k/include)
include_directories(.)

set(MOD_PY_ARRAY)
set(MICROPY_PY_IO)
set(MICROPY_PY_IO_FILEIO)
set(MICROPY_PY_UZLIB)
set(MICROPY_PY_UJSON)
set(MICROPY_HELPER_REPL)

add_executable(micropython ${SOURCE_FILES})