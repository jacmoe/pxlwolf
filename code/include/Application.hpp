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
#include "SDLDeleter.hpp"

class Application
{
    public:
        Application();
        virtual ~Application();

        bool init(const std::string title, const int width, const int height, const int scale = 1);
        void run();

		// Called once on application startup, use to load your resources
		virtual bool OnUserCreate();
		// Called every frame, and provides you with a time per frame value
		virtual bool OnUserUpdate(double fDeltaTime);
		// Called once on application termination, so you can be one clean coder
		virtual bool OnUserDestroy();

		virtual bool OnUserRender();

        virtual bool write_text(const std::string text);

        std::string font_name;
        int font_size;
        SDL_Color font_color;

        TTF_Font* m_font;
        std::unique_ptr<SDL_Texture, SDLDeleter> m_font_texture;

        std::unique_ptr<SDL_Window, SDLDeleter> m_window;
        std::unique_ptr<SDL_Renderer, SDLDeleter> m_renderer;

    private:
        bool running;

        SDL_Event e_;

        std::shared_ptr<spdlog::logger> m_pxllogger;
        
        void setup_working_directory();
        void setup_logging();
        void load_font();

        void event();
        void update();
        void render();
};
