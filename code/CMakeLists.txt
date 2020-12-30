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
#   MIT License
#
set(TARGET_NAME pxlwolf)

set(Sources
    ./src/dbg_console.cpp
    ./src/game_engine.cpp
    ./src/main.cpp
    ./src/pixel_render.cpp
    ./src/raycaster_engine.cpp
    ./src/system.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/dbg_console.hpp
    ../${IncludeDir}/DG_misc.hpp
    ../${IncludeDir}/game_engine.hpp
    ../${IncludeDir}/pixel_render.hpp
    ../${IncludeDir}/raycaster_engine.hpp
    ../${IncludeDir}/SDL_stbimage.hpp
    ../${IncludeDir}/stb_image.hpp
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

target_link_libraries(pxlwolf optimized ${SDL2_LIBS_RELEASE} debug ${SDL2_LIBS_DEBUG})
