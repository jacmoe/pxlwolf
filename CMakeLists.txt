# This file is part of the
#   ___ _       __    __      _  __ 
#  / _ \ |_  __/ / /\ \ \___ | |/ _|
# / /_)/ \ \/ /\ \/  \/ / _ \| | |_ 
#/ ___/| |>  <  \  /\  / (_) | |  _|
#\/    |_/_/\_\  \/  \/ \___/|_|_|  
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

project(pixelwolf)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} CMakeFiles/)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(IncludeDir code/include/)
include_directories(${IncludeDir})
include_directories(${IncludeDir}/3rd)
include_directories(${IncludeDir}/nasl)

set(THREADS_PREFER_PTHREAD_FLAD ON)
find_package(Threads REQUIRED)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

if (UNIX)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
endif (UNIX)

if (WIN32)
    include_directories(${WinSDK})
endif (WIN32)


add_subdirectory(code)
