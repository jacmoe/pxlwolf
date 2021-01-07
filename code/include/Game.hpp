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

#include <string>
#include <vector>
#include <memory>
#include <SDL.h>

#include "Camera.hpp"
#include "SDLRenderer.hpp"
#include "RayCaster.hpp"
#include "SDLDeleter.hpp"

typedef std::vector< std::vector<int> > Map;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

static const float BASE_MOVEMENT_SPEED = 0.035f;
static const float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED + 0.04f;
static const float CURSOR_TURN_SPEED = 0.03f;

class Game
{
    public:
        Game();
        ~Game();

        void init();
        void run();

    private:
        bool running_;
        Map map_;
        bool overview_map_on;
        float movement_speed_;

        std::unique_ptr<SDLRenderer> renderer_;
        RayCaster raycaster_;
        Camera camera_;

        SDL_Event e_;
        std::unique_ptr<SDL_Texture, SDLDeleter> top_texture_;

        Map loadMap(const std::string& path);

        void event();
        void update();
        void render();

        void drawMap();
};
