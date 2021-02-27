
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
![Clutter creates an atmosphere][clutter]

![Static sprites][sprites]

![Textured floor and ceiling][corridor]

![PixelWolf][pxlwolf]


### Level Editing in LDtk
Pxlwolf uses the [LDtk Level Designer Toolkit][ldtk_link] to create and edit levels.
![LDTK][ldtk]
First four levels of Wolfenstein 3D.
![Wolfenstein Levels][ldtklevels]
Taking care of the details.
![Static sprites and pickups][detailing]

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
PixelWolf is being developed on Linux and Windows.

Compiled using MSVC 2019 on Windows, and GCC 9.3 on Linux.

Uses Visual Studio on Windows, and CMake on Linux.

### External dependencies
|Library|Description|License|
|-------|-----------|-------|
|[Allegro 5.2][allegro]|A game programming library|Zlib license|

#### Windows
The easiest way to get Allegro on Windows is via Nuget, which is why this project uses Visual Studio for Windows.

The PixelWolf solution references two projects, Lua and PhysicsFS, which should be generated using CMake before opening the project.

Use the CMake script in `3rdparty` and choose `3rdparty/vsbuild` as the "where to build the binaries" folder.

Make sure `win32` is chosen as the architecture, otherwise CMake will choose `x64` (and that is not going  to work).

After having generated the two projects, you can now open the PixelWolf solution. Maybe in the future, there will be a better way.


#### Linux
Allegro should be available from your package manager. On Ubuntu, perform a simple ```sudo apt-get install liballegro5-dev```


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


## Credits

This project is using code from the [c-raycaster project][cray] - the raycaster and a lot of the pixel related functionality is borrowed from the project. It also uses code from Herrecito's [2.5D Raycaster engine][herrecito] in C. And the overall structure of the project is lifted from Balantkissdev's [Raycaster engine][balantkiss].


[pxlwolf]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/Screenshot.jpg "Screenshot of PixelWolf"
[corridor]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/corridor.png "Textured wall and ceiling"
[detailing]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/detailing.png "Taking care of the details"
[clutter]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/clutter.png "Creating an atmosphere"
[sprites]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/sprites.png "Static sprites"
[ldtk]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/LevelEditing.jpg "Level editing in LDTK"
[ldtklevels]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/ldtklevels.png "First four levels of Wolf3d"

[ldtk_link]: https://ldtk.io/ "LDtk Level Designer Toolkit"

[herrecito]: https://github.com/herrecito/engine "Herrecito's 2.5D Raycaster engine in C"
[balantkiss]: https://github.com/balintkissdev/raycaster-engine "Balantkissdev's Raycaster engine"
[cray]: https://github.com/ckinvents/c-raycaster "c-raycaster project by ckinvents"

[allegro]: https://liballeg.org/ "A game programming library"
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
