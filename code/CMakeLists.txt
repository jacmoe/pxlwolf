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
set(TARGET_NAME pxlwolf)

set(Sources
    ./src/buffer.cpp
    ./src/collision.cpp
    ./src/color.cpp
    ./src/dbg_console.cpp
    ./src/draw.cpp
    ./src/geometry.cpp
    ./src/main.cpp
    ./src/map.cpp
    ./src/sprites.cpp
    ./src/system.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/buffer.hpp
    ../${IncludeDir}/collision.hpp
    ../${IncludeDir}/color.hpp
    ../${IncludeDir}/dbg_console.hpp
    ../${IncludeDir}/dbg.hpp
    ../${IncludeDir}/defs.hpp
    ../${IncludeDir}/draw.hpp
    ../${IncludeDir}/geometry.hpp
    ../${IncludeDir}/map.hpp
    ../${IncludeDir}/sprites.hpp
    ../${IncludeDir}/system.hpp
    ../${IncludeDir}/utils.hpp
)

if (WIN32)
    add_executable(${TARGET_NAME}
        WIN32
        ${Sources}
        ${Includes}
    )
endif (WIN32)
if (UNIX)
    add_executable(${TARGET_NAME}
        ${Sources}
        ${Includes}
    )
endif (UNIX)


if (UNIX)
    target_link_libraries(${TARGET_NAME} stdc++fs)
    target_link_libraries(${TARGET_NAME} GL)
    target_link_libraries(${TARGET_NAME} png)

    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    target_link_libraries(${TARGET_NAME}
            ${X11_LIBRARIES}
            PNG::PNG
            )
endif (UNIX)


#target_link_libraries(pxlwolf ${SDL2_LIBS_RELEASE})
target_link_libraries(pxlwolf debug ${SDL2_LIBS_DEBUG})
