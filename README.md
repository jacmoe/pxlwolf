
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

**ACHTUNG!** Project is currently being ported from SDL to SFML, and the raycaster hasn't beeen ported (yet)

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
### Level Editing in LDtk
Pxlwolf uses the [LDtk Level Designer Toolkit][ldtk_link] to create and edit levels.
![LDTK][ldtk]


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

Everything, except sfml-audio, is linked statically on Windows.

Compiled using MSVC 2019 on Windows, and GCC 9.3 on Linux.

### External dependencies
|Library|Description|License|
|-------|-----------|-------|
|[SFML 2.5.1][sfml]|Simple and Fast Multimedia Library|zlib license|

### Bundled dependencies
|Library|Description|License|
|-------|-----------|-------|
|[cereal][cereal]|C++11 library for serialization|BSD license|
|[Lua 5.4][lua]|Scripting language|zlib License|
|[PhysicsFS ][physfs]|Archive support for games|zlib license|
|[PhysFS++][physpp]|C++ wrapper for PhysicsFS|zlib license|
|[RapidJSON][rapidjson]|A fast JSON parser/generator for C++|MIT license|
|[SelbaWard][selba]|A collection of drawable C++ objects for use with SFML|zlib license|
|[Sol][sol]|C++ binding library to Lua|MIT License|
|[spdlog][spdlog]|Logging library|MIT license|
|[SFML-StateMachine][state]|State manager for SFML|MIT license|
|[Thor][thor]|SFML extension library|zlib license|
|[DG Snippets][dg]|Misc public domain functions|Public domain license|


## Credits

This project is using code from the [c-raycaster project][cray] - the raycaster and a lot of the pixel related functionality is borrowed from the project. It also uses code from Herrecito's [2.5D Raycaster engine][herrecito] in C. And the overall structure of the project is lifted from Balantkissdev's [Raycaster engine][balantkiss].


[pxlwolf]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/Screenshot.jpg "Screenshot of PixelWolf"
[ldtk]: https://github.com/jacmoe/pxlwolf/raw/main/screenshots/LevelEditing.jpg "Level editing in LDTK"
[ldtk_link]: https://ldtk.io/ "LDtk Level Designer Toolkit"

[herrecito]: https://github.com/herrecito/engine "Herrecito's 2.5D Raycaster engine in C"
[balantkiss]: https://github.com/balintkissdev/raycaster-engine "Balantkissdev's Raycaster engine"
[cray]: https://github.com/ckinvents/c-raycaster "c-raycaster project by ckinvents"

[sfml]: https://www.sfml-dev.org/download/sfml/2.5.1/ "Simple and Fast Media Library - 2D graphics C++ library"
[cereal]: https://uscilab.github.io/cereal/ "C++11 library for serialization"
[lua]: https://www.lua.org/download.html "Scripting language"
[physfs]: https://www.icculus.org/physfs/ "Archive support for games"
[physpp]: https://github.com/kahowell/physfs-cpp "C++ wrapper for PhysicsFS"
[rapidjson]: https://rapidjson.org/ "A fast JSON parser/generator for C++"
[selba]: https://github.com/Hapaxia/SelbaWard "A collection of drawable C++ objects for use with SFML"
[sol]: https://github.com/ThePhD/sol "C++ binding library to Lua"
[spdlog]: https://github.com/gabime/spdlog "Logging library"
[state]: https://github.com/astewartau/SFML-StateMachine "State manager for SFML"
[thor]: https://github.com/Bromeon/Thor "SFML extension library"
[dg]: https://github.com/DanielGibson/Snippets/ "Misc public domain functions"
