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
set(TARGET_NAME plxwolf)

set(Sources
    ./src/main.cpp
    ./src/utils.cpp
    ./src/3rd/my_basic.c
    ./src/nasl/nasl_buffer_script.cpp
    ./src/nasl/nasl_buffer.cpp
    ./src/nasl/nasl_color_script.cpp
    ./src/nasl/nasl_color.cpp
    ./src/nasl/nasl_draw_script.cpp
    ./src/nasl/nasl_draw.cpp
    ./src/nasl/nasl_geometry.cpp
    ./src/nasl/nasl_graphics_script.cpp
    ./src/nasl/nasl_graphics.cpp
    ./src/nasl/nasl_image_script.cpp
    ./src/nasl/nasl_image.cpp
    ./src/nasl/nasl_script.cpp
    ./src/nasl/nasl_sprite_script.cpp
    ./src/nasl/nasl_sprite.cpp
)

set(Includes
    ../${IncludeDir}/utils.h
    ../${IncludeDir}/3rd/my_basic.h
    ../${IncludeDir}/3rd/stb_image.h
    ../${IncludeDir}/3rd/sysfont.h
    ../${IncludeDir}/nasl/nasl_buffer.h
    ../${IncludeDir}/nasl/nasl_color.h
    ../${IncludeDir}/nasl/nasl_dbg.h
    ../${IncludeDir}/nasl/nasl_defs.h
    ../${IncludeDir}/nasl/nasl_draw.h
    ../${IncludeDir}/nasl/nasl_geometry.h
    ../${IncludeDir}/nasl/nasl_graphics.h
    ../${IncludeDir}/nasl/nasl_image.h
    ../${IncludeDir}/nasl/nasl_script.h
    ../${IncludeDir}/nasl/nasl_sprite.h
)

if (WIN32)
    add_executable(${TARGET_NAME}
        #WIN32
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

target_link_libraries(${TARGET_NAME}
        Threads::Threads
        GLEW::GLEW
        glfw
        ${OPENGL_LIBRARIES}
        )

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
    set(SFML_STATIC_LIBRARIES FALSE)
    target_link_libraries(${TARGET_NAME}
            ${X11_LIBRARIES}
            PNG::PNG
            )
endif (UNIX)
if (WIN32)
    target_link_libraries(${TARGET_NAME}
            ${WinSDK}
            )
endif (WIN32)
