# This file is part of the
# ██████╗ ██╗     ██╗  ██╗██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗██║     ╚██╗██╔╝██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝██║      ╚███╔╝ ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝ ██║      ██╔██╗ ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ███████╗██╔╝ ██╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚══════╝╚═╝  ╚═╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
# project
# https://github.com/jacmoe/pxlwolf
#
# (c) 2020 Jacob Moena
#
# BSD 2-Clause "Simplified" License
#
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)

set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/sdl2)
set(sdl2-image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/sdl2-image)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include/SDL2)

set(SDL2_LIBS_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/SDL2.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/SDL2_image.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/manual-link/SDL2main.lib
)
set(SDL2_LIBS_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/SDL2d.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/SDL2_image.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/manual-link/SDL2maind.lib
)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)


set(IncludeDir code/include/) # Change include/ to your include directory

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${IncludeDir}
)

project(pixelwolf)

add_subdirectory(code)
