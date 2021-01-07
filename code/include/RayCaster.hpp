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

#include "WallTypes.hpp"
#include "MathLib.hpp"
#include "Camera.hpp"

#include <SDL.h>

#include <vector>

class SDLRenderer;

class RayCaster
{
    public:
        RayCaster();

        void init(
            const std::vector< std::vector<int> > map,
            const int width,
            const int height
        );

        void drawTop(SDLRenderer* renderer);
        void drawTop(SDLRenderer* renderer, SDL_Texture* top_texture);
        void drawBottom(SDLRenderer* renderer);
        void drawWalls(SDLRenderer* renderer, const Camera& camera);

    private:
        std::vector< std::vector<int> > map_;
        int width_, height_;

        void drawPlainColoredStripe(
            SDLRenderer* renderer,
            const int x,
            const mymath::Point2d<int>& square_on_map,
            const int draw_start, const int draw_end,
            const int side
        );
};
