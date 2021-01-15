
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
Wolfenstein-like raycast engine

**ACHTUNG!** Project is currently being ported from SDL to SFML, and the raycaster doesn't work (yet)


## Screenshots
### Main window 
![PixelWolf][pxlwolf]
### Level Editing in LDtk
Pxlwolf uses the [LDtk Level Designer Toolkit][ldtk_link] to create and edit levels.
![LDTK][ldtk]


## Dependencies
### External dependencies
[SFML 2.5.1][sfml]
### Bundled dependencies
|Library|Description|License|
|-------|-----------|-------|
|[Sol][sol]|C++ binding library to Lua|MIT License|
|[Lua 5.4][lua]|Scripting language|zlib License|
|[spdlog][spdlog]|Logging library|MIT License|
|[PhysicsFS ][physfs]|Archive support for games|zlib license|

## Credits

This project is using code from the [c-raycaster project][cray] - the raycaster and a lot of the pixel related functionality is borrowed from the project. It also uses code from Herrecito's [2.5D Raycaster engine][herrecito] in C. And the overall structure of the project is lifted from Balantkissdev's [Raycaster engine][balantkiss].


[pxlwolf]: https://github.com/jacmoe/pxlwolf/raw/main/Screenshot.jpg "Screenshot of PixelWolf"
[ldtk]: https://github.com/jacmoe/pxlwolf/raw/main/LevelEditing.jpg "Level editing in LDTK"
[ldtk_link]: https://ldtk.io/ "LDtk Level Designer Toolkit"

[herrecito]: https://github.com/herrecito/engine "Herrecito's 2.5D Raycaster engine in C"
[balantkiss]: https://github.com/balintkissdev/raycaster-engine "Balantkissdev's Raycaster engine"
[cray]: https://github.com/ckinvents/c-raycaster "c-raycaster project by ckinvents"

[sfml]: https://www.sfml-dev.org/download/sfml/2.5.1/ "Simple and Fast Media Library - 2D graphics C++ library"
[sol]: https://github.com/ThePhD/sol "C++ binding library to Lua"
[lua]: https://www.lua.org/download.html "Scripting language"
[spdlog]: https://github.com/gabime/spdlog "Logging library"
[physfs]: https://www.icculus.org/physfs/ "Archive support for games"
