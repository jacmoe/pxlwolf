set(LUA_LIB_SRCS 
    "src/lapi.c"
    "src/lcode.c"
    "src/lctype.c"
    "src/ldebug.c"
    "src/ldo.c"
    "src/ldump.c"
    "src/lfunc.c"
    "src/lgc.c"
    "src/llex.c"
    "src/lmem.c"
    "src/lobject.c"
    "src/lopcodes.c"
    "src/lparser.c"
    "src/lstate.c"
    "src/lstring.c"
    "src/ltable.c"
    "src/ltm.c"
    "src/lundump.c"
    "src/lvm.c"
    "src/lzio.c"
    "src/lauxlib.c"
    "src/lbaselib.c"
    "src/lcorolib.c"
    "src/ldblib.c"
    "src/liolib.c"
    "src/lmathlib.c"
    "src/loadlib.c"
    "src/loslib.c"
    "src/lstrlib.c"
    "src/ltablib.c"
    "src/lutf8lib.c"
    "src/linit.c"
)
#TODO: Redo this with find_package
if(UNIX)
    find_library(LIBM m)
    if(NOT LIBM)
        message(FATAL_ERROR "libm not found and requred by lua")
    endif()
endif(UNIX)

add_library(lua STATIC ${LUA_LIB_SRCS})

set_target_properties(lua PROPERTIES OUTPUT_NAME "lua")

target_include_directories(lua PUBLIC "include")

if(UNIX)
    target_link_libraries(lua INTERFACE ${LIBM})
    set(LUA_DEFINITIONS)
    list(APPEND LUA_DEFINITIONS LUA_USE_POSIX)
    if(LUA_SUPPORT_DL)
        target_compile_definitions(lua_static PRIVATE "LUA_USE_DLOPEN")
        target_link_libraries(lua_static INTERFACE dl)
    endif()
    target_compile_definitions(lua_static PUBLIC ${LUA_DEFINITIONS})
    target_compile_options(lua_static PRIVATE "-Wall" "-Wextra")
endif(UNIX)
