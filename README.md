
    # The
    # ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
    # ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
    # ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗  
    # ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝  
    # ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║     
    # ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
    # project
    # https://github.com/jacmoe/pxlwolf
    #
    # (c) 2020 - 2021 Jacob Moena
    #
    # MIT License
    #

# pxlwolf
Seriously overengineered Wolfenstein-like raycast engine

## TOC

- [Screenshots](#Screenshots)
  - [PixelWolf](#PixelWolf)
  - [Level Editing in LDtk](#Level-Editing-in-LDtk)
- [Goals](#Goals)
- [Features](#Goals)
  - [Implemented features](#Implemented-features)
  - [Planned features](#Planned-features)
- [Dependencies](#Dependencies)
  - [External dependencies](#External-dependencies)
  - [Bundled dependencies](#Bundled-dependencies)
- [Credits](#Credits)


## Screenshots
### PixelWolf 
![PixelWolf][pxlwolf]

![Textured floor and ceiling][corridor]

### Level Editing in LDtk
Pxlwolf uses the [LDtk Level Designer Toolkit][ldtk_link] to create and edit levels.
![LDTK][ldtk]
First four levels of Wolfenstein 3D.
![Wolfenstein Levels][ldtklevels]

## Goals
- Explore raycasting, including height-mapping and animated tiles.
- Learn how to use modern C++ features, and move from C++98 to C++17 and beyond.
- Create a complete game, and not just a tech demo.
- Use third-party code whenever possible, but keep things simple.

## Features
Here I will write the features of PixelWolf down as I implement them.
### Implemented
### Planned

## Dependencies
PixelWolf is being developed on Linux and Windows, and the build system is based on CMake.

Compiled using MSVC 2019 on Windows, and GCC 9.3 on Linux.

### External dependencies
<!-- |Library|Description|License|
|-------|-----------|-------|
|[OpenAL 1.1][openal]|Cross-platform 3D audio API|LGPL/Proprietary license| -->

### Bundled dependencies
|Library|Description|License|
|-------|-----------|-------|
|[cereal][cereal]|C++11 library for serialization|BSD license|
|[enTT][entt]|C++ entity component system|MIT license|
|[linalg][linalg]|Single header vector math library for C++|Public domain|
|[Lua 5.4][lua]|Scripting language|zlib License|
|[PhysicsFS ][physfs]|Archive support for games|zlib license|
|[PhysFS++][physpp]|C++ wrapper for PhysicsFS|zlib license|
|[RapidJSON][rapidjson]|A fast JSON parser/generator for C++|MIT license|
|[Sol][sol]|A C++11 Lua wrapper|MIT License|
|[spdlog][spdlog]|Logging library|MIT license|
|[stb_image][stbimage]|Image loader|Public domain|
|[toml11][toml11]|TOML for Modern C++|MIT license|


## Credits

This project is using code from the [c-raycaster project][cray] - the raycaster and a lot of the pixel related functionality is borrowed from the project. It also uses code from Herrecito's [2.5D Raycaster engine][herrecito] in C. And the overall structure of the project is lifted from Balantkissdev's [Raycaster engine][balantkiss].


[pxlwolf]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/Screenshot.jpg "Screenshot of PixelWolf"
[corridor]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/corridor.jpg "Textured wall and ceiling"
[ldtk]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/LevelEditing.jpg "Level editing in LDTK"
[ldtklevels]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/ldtklevels.png "First four levels of Wolf3d"

[ldtk_link]: https://ldtk.io/ "LDtk Level Designer Toolkit"

[herrecito]: https://github.com/herrecito/engine "Herrecito's 2.5D Raycaster engine in C"
[balantkiss]: https://github.com/balintkissdev/raycaster-engine "Balantkissdev's Raycaster engine"
[cray]: https://github.com/ckinvents/c-raycaster "c-raycaster project by ckinvents"

[openal]: https://www.openal.org/downloads/ "Cross-platform 3D audio API"
[cereal]: https://uscilab.github.io/cereal/ "C++11 library for serialization"
[entt]: https://github.com/skypjack/entt "C++ entity component system"
[linalg]: https://github.com/sgorsten/linalg "Single header vector math library for C++"
[lua]: https://www.lua.org/download.html "Scripting language"
[physfs]: https://www.icculus.org/physfs/ "Archive support for games"
[physpp]: https://github.com/kahowell/physfs-cpp "C++ wrapper for PhysicsFS"
[rapidjson]: https://rapidjson.org/ "A fast JSON parser/generator for C++"
[sol]: https://github.com/ThePhD/sol "A C++11 Lua wrapper"
[spdlog]: https://github.com/gabime/spdlog "Logging library"
[stbimage]: https://github.com/nothings/stb "Image loader"
[toml11]: https://github.com/ToruNiina/toml11 "TOML for Modern C++"
