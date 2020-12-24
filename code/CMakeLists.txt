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
    ./src/LDtkLoader/DataTypes.cpp
    ./src/LDtkLoader/Entity.cpp
    ./src/LDtkLoader/EntityDef.cpp
    ./src/LDtkLoader/Enum.cpp
    ./src/LDtkLoader/Layer.cpp
    ./src/LDtkLoader/LayerDef.cpp
    ./src/LDtkLoader/Level.cpp
    ./src/LDtkLoader/Tileset.cpp
    ./src/LDtkLoader/Utils.cpp
    ./src/LDtkLoader/World.cpp
)

set(Includes
    ../${IncludeDir}/olcPixelGameEngine.h
    ../${IncludeDir}/utils.h
    ../${IncludeDir}/LDtkLoader/thirdparty/json.hpp
    ../${IncludeDir}/LDtkLoader/DataTypes.hpp
    ../${IncludeDir}/LDtkLoader/Entity.hpp
    ../${IncludeDir}/LDtkLoader/EntityDef.hpp
    ../${IncludeDir}/LDtkLoader/Enum.hpp
    ../${IncludeDir}/LDtkLoader/Layer.hpp
    ../${IncludeDir}/LDtkLoader/LayerDef.hpp
    ../${IncludeDir}/LDtkLoader/Level.hpp
    ../${IncludeDir}/LDtkLoader/Tileset.hpp
    ../${IncludeDir}/LDtkLoader/Utils.hpp
    ../${IncludeDir}/LDtkLoader/World.hpp
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

target_link_libraries(${TARGET_NAME}
        Threads::Threads
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
