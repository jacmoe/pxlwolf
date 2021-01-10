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
#include "spdlog/spdlog.h"
#include <SFML/Graphics.hpp>

class Application
{
    public:
        Application();
        virtual ~Application();

        bool init(const std::string title, const int width, const int height, const int scale = 1);
        void run();

		virtual bool OnUserCreate();
		virtual bool OnUserUpdate(double fDeltaTime);
		virtual bool OnUserRender();
		virtual bool OnUserDestroy();

        virtual bool write_text(const std::string text);

        std::string font_name;
        int font_size;
        sf::Color font_color;
        float m_scale;
        int m_width;
        int m_height;

        std::unique_ptr<sf::RenderWindow> m_renderwindow;
        sf::Sprite m_rendersprite;
        sf::Texture m_rendertexture;

    private:
        bool running;

        std::shared_ptr<spdlog::logger> m_pxllogger;
        
        void setup_working_directory();
        void setup_logging();
        bool load_font();

        void event();
        void update();
        void render();
};
