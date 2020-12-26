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

if(WIN32)
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/sfml)
    set(SFML_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include)
    set(SFML_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/sfml-graphics.lib 
        ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/sfml-system.lib 
        ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/sfml-window.lib )
endif(WIN32)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.4.2 REQUIRED system window graphics)

include_directories(
    ${SFML_INCLUDE_DIRS}
)

project(pixelwolf)

add_subdirectory(code)
