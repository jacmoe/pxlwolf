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

#include "raylib.h"

// #include "Pixelator.hpp"

class Application
{
public:
    Application();
    virtual ~Application();

    bool init(const std::string title, int width, int height, float scale = 1.0f, const bool fullscreen = false);
    void run();

protected:
    virtual bool OnUserCreate();
    virtual bool OnUserUpdate(double elapsedTime);
    virtual bool OnUserRender();
    virtual bool OnUserDestroy();

    float m_scale;
    int m_width;
    int m_height;
    bool m_fullscreen;

    RenderTexture2D m_render_texture;

private:
    std::string m_title;
    bool m_running;
    Font m_font;

    Vector2 m_mouse_position;
    Vector2 m_virtual_mouse_position = { 0 };

    float m_framebuffer_scale;

    void event();
    void update(double elapsedTime);
    void render();
    void toggle_fullscreen();
};
