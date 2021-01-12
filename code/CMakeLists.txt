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
set(TARGET_NAME pxlwolf)

set(Sources
    ./src/Application.cpp
    ./src/dbg_console.cpp
    ./src/Game.cpp
    ./src/ImageAtlas.cpp
    ./src/Main.cpp
    ./src/Map.cpp
    #./src/Mechanics.cpp
    ./src/physfs.cpp
    ./src/Pixelator.cpp
    #./src/RayCaster.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/Application.hpp
    ../${IncludeDir}/dbg_console.hpp
    ../${IncludeDir}/DG_misc.hpp
    ../${IncludeDir}/Game.hpp
    ../${IncludeDir}/ImageAtlas.hpp
    ../${IncludeDir}/lua_main.hpp
    ../${IncludeDir}/Map.hpp
    #../${IncludeDir}/Mechanics.hpp
    #../${IncludeDir}/MathLib.hpp
    ../${IncludeDir}/physfs.hpp
    ../${IncludeDir}/Pixelator.hpp
    #../${IncludeDir}/RayCaster.hpp
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

target_link_libraries(pxlwolf lua  physfs-static sfml-graphics sfml-system sfml-main)
