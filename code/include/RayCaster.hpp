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

class IRenderer;

class RayCaster
{
    public:
        RayCaster(
            const std::vector< std::vector<int> >& map,
            const int width,
            const int height
        );

        void drawTop(IRenderer* renderer);
        void drawTop(IRenderer* renderer, SDL_Texture* top_texture);
        void drawBottom(IRenderer* renderer);
        void drawWalls(IRenderer* renderer, const Camera& camera);

    private:
        const std::vector< std::vector<int> >& map_;
        int width_, height_;

        void drawPlainColoredStripe(
            IRenderer* renderer,
            const int x,
            const mymath::Point2d<int>& square_on_map,
            const int draw_start, const int draw_end,
            const int side
        );
};
