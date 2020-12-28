# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 Jacob Moena
#
#   BSD 2-Clause "Simplified" License
#
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")


set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/packages/installed/x64-windows-static/share/glew/")

project(pixelwolf)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

find_library(SDL2_LIBRARY SDL2)
find_library(SDL2_IMAGE SDL2_image)
find_library(SDL2_MAIN SDL2main)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAD ON)
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
endif(UNIX)

if(${GLEW_FOUND})
  add_definitions(-DUSE_GLEW)
endif()

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(WIN32)
include_directories(${WinSDK})
set(SDL2_DIR ${_VCPKG_INSTALLED_DIR}x64-windows/share/sdl2)
set(sdl2-image_DIR ${_VCPKG_INSTALLED_DIR}/x64-windows/share/sdl2-image)
set(SDL2_INCLUDE_DIRS ${_VCPKG_INSTALLED_DIR}/x64-windows/include/SDL2)
set(GL_INCLUDE_DIRS ${_VCPKG_INSTALLED_DIR}/x64-windows/include/gl)
set(GLEW_STATIC ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/glew32.lib)
set(GLEW_STATIC_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/glew32d.lib)
endif(WIN32)

if(UNIX)
set(SDL2_DIR ${_VCPKG_INSTALLED_DIR}/x64-linux/share/sdl2)
set(sdl2-image_DIR ${_VCPKG_INSTALLED_DIR}/x64-linux/share/sdl2-image)
set(SDL2_INCLUDE_DIRS ${_VCPKG_INSTALLED_DIR}/x64-linux/include/SDL2)
set(GL_INCLUDE_DIRS ${_VCPKG_INSTALLED_DIR}/x64-linux/include/GL)

set(SDL2_LIBS_RELEASE
    ${_VCPKG_INSTALLED_DIR}/x64-linux/lib/libGLEW.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/lib/libSDL2.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/lib/libSDL2_image.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/lib/libSDL2main.a
    dl
    png
)
set(SDL2_LIBS_DEBUG
    ${_VCPKG_INSTALLED_DIR}/x64-linux/debug/lib/libGLEWd.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/debug/lib/libSDL2d.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/debug/lib/libSDL2_image.a
    ${_VCPKG_INSTALLED_DIR}/x64-linux/debug/lib/libSDL2maind.a
    dl
    png
)
endif(UNIX)

set(IncludeDir code/include/) # Change include/ to your include directory

include_directories(
    ${GL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${IncludeDir}
)

add_subdirectory(code)
