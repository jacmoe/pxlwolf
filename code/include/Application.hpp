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
#include "spdlog/spdlog.h"
#include "SDLRenderer.hpp"
#include "SDLDeleter.hpp"

class Application
{
    public:
        Application();
        virtual ~Application();

        bool init(const int width, const int height, const std::string title);
        void run();

		// Called once on application startup, use to load your resources
		virtual bool OnUserCreate();
		// Called every frame, and provides you with a time per frame value
		virtual bool OnUserUpdate(double fDeltaTime);
		// Called once on application termination, so you can be one clean coder
		virtual bool OnUserDestroy();

    private:
        bool running_;

        SDL_Event e_;
        
        std::unique_ptr<SDLRenderer> renderer_;

        std::shared_ptr<spdlog::logger> pxllogger;
        
        void setup_working_directory();
        void setup_logging();

        void event();
        void update();
        void render();
};
