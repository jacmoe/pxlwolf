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
#include "Pixelator.hpp"
#include "Timer.hpp"

class Application
{
public:
    Application();
    virtual ~Application();

    bool init(const std::string title, int width, int height, float scale = 1.0f, const bool fullscreen = false);
    void run();

protected:
    virtual bool OnUserCreate();
    virtual bool OnUserUpdate(double fDeltaTime);
    virtual bool OnUserRender();
    virtual bool OnUserDestroy();

    virtual bool write_text(const std::string text);

    uint32_t get_fps() { return static_cast<uint32_t>(std::round(m_average_fps)); }

    float m_scale;
    int m_width;
    int m_height;
    bool m_fullscreen;
    bool m_show_map;
    float m_average_fps;

    std::string font_name;
    int font_size;
    SDL_Color font_color;

    TTF_Font* m_font;
    std::unique_ptr<SDL_Texture, utility::SDLDeleter> m_font_texture;
    std::unique_ptr<SDL_Texture, utility::SDLDeleter> m_render_texture;

    std::unique_ptr<SDL_Window, utility::SDLDeleter> m_window;
    std::unique_ptr<SDL_Renderer, utility::SDLDeleter> m_renderer;

    std::shared_ptr<Pixelator> m_pixelator;

private:
    std::string m_title;
    bool m_running;
    bool m_show_fps;
    bool m_should_exit;
    utility::Timer m_fps_timer;

    SDL_Event e_;

    void event();
    void update(double elapsedTime);
    void render();
    void toggle_fullscreen();
    bool load_font();
};
