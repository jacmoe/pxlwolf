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
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#
cmake_minimum_required(VERSION 3.7)

project(pixelwolf)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAD ON)
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
endif(UNIX)

if(WIN32)
set(SFML_DIR C:/SFML/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS system window graphics audio main)
endif(WIN32)

if(UNIX)
find_package(SFML 2 COMPONENTS system window graphics audio)
endif(UNIX)

if(WIN32)
include_directories(${WinSDK})
set(SFML_INCLUDE_DIRS C:/SFML/include)
endif(WIN32)

add_subdirectory(3rdparty/Lua)

add_subdirectory(3rdparty/physfs)

set(THOR_SHARED_LIBS FALSE)
add_subdirectory(3rdparty/thor)

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(SPDLOG_INCLUDE_DIR ${3RD_PARTY_DIR}/spdlog/include)
set(SOL_INCLUDE_DIR ${3RD_PARTY_DIR}/sol2)
set(ENTT_INCLUDE_DIR ${3RD_PARTY_DIR}/entt)
set(LUA_INCLUDE_DIR ${3RD_PARTY_DIR}/Lua/include)
set(PHYSFS_INCLUDE_DIR ${3RD_PARTY_DIR}/physfs/src)
set(THOR_INCLUDE_DIR ${3RD_PARTY_DIR}/thor/include)
set(AURORA_INCLUDE_DIR ${3RD_PARTY_DIR}/thor/extlibs/aurora/include)
set(SelbaWardDir ${3RD_PARTY_DIR}/SelbaWard)
set(IncludeDir code/include/)

include_directories(
    ${3RD_PARTY_DIR}
    ${SPDLOG_INCLUDE_DIR}
    ${ENTT_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ${THOR_INCLUDE_DIR}
    ${AURORA_INCLUDE_DIR}
    ${SFML_INCLUDE_DIRS}
    ${SelbaWardDir}
    ${IncludeDir}
)

add_subdirectory(code)
