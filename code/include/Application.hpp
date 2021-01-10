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

class Application : private sf::NonCopyable
{
    public:
        Application();
        virtual ~Application();

        bool init(const std::string title, const int width, const int height, const float scale = 1.0f);
        void run();

		virtual bool OnUserCreate();
		virtual bool OnUserUpdate(sf::Time elapsedTime);
		virtual bool OnUserRender();
		virtual bool OnUserDestroy();

        virtual bool write_text(const std::string text);

        std::string m_font_name;
        int m_font_size;
        sf::Color m_font_color;
        sf::Font m_font;
        float m_scale;
        int m_width;
        int m_height;

        std::unique_ptr<sf::RenderWindow> m_renderwindow;
        sf::Texture m_rendertexture;

    private:
		static const sf::Time m_time_per_frame;
        bool running;
        std::string m_title;

        std::shared_ptr<spdlog::logger> m_pxllogger;
        sf::Sprite m_rendersprite;
        sf::Text m_text;
        
        void setup_working_directory();
        void setup_logging();
        bool load_font();

        void event();
        void update();
        void render();
};
