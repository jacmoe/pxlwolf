/*# This file is part of the
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
#*/
#pragma once
/*
* Coded by Balint Kiss for his Raycaster-engine project at https://github.com/balintkissdev/raycaster-engine
* Copyright (c) 2012-2021 Balint Kiss - MIT licensed.
* Modified slightly by Jacob Moena (added the TTF_Font)
*/

#include <SDL.h>
#include <SDL_ttf.h>

namespace utility {
    struct SDLDeleter
    {
        void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
        void operator()(SDL_Renderer* render) { SDL_DestroyRenderer(render); }
        void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
        void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
        void operator()(TTF_Font* font) { TTF_CloseFont(font); }
    };
}
