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

set(TARGET_NAME plxwolf)

set(Sources
    ./src/main.cpp
)

#set(Includes
#    ../${IncludeDir}/utils.h
#)

if (WIN32)
    add_executable(${TARGET_NAME}
        #WIN32
        ${Sources}
        #${Includes}
    )
endif (WIN32)
if (UNIX)
    add_executable(${TARGET_NAME}
        ${Sources}
        #${Includes}
    )
endif (UNIX)

target_link_libraries(plxwolf PRIVATE ${SFML_LIBS} FLAC OpenAL OpenGL Vorbis)
