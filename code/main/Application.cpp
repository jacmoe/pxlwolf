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
#include "Application.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

// Clamp Vector2 value with min and max and return a new vector2
// NOTE: Required for virtual mouse, to clamp inside virtual game size
Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max)
{
    Vector2 result = value;
    result.x = (result.x > max.x)? max.x : result.x;
    result.x = (result.x < min.x)? min.x : result.x;
    result.y = (result.y > max.y)? max.y : result.y;
    result.y = (result.y < min.y)? min.y : result.y;
    return result;
}


Application::Application()
    : m_scale(0)
    , m_width(0)
    , m_height(0)
    , m_fullscreen(false)
    , m_title("")
    , m_running(false)
    , m_font()
{}

Application::~Application()
{
    TraceLog(LOG_INFO,"PixelWolf shutdown.");
}

bool Application::OnUserCreate()
{
    return true;
}

bool Application::OnUserUpdate(double elapsedTime)
{
    return true;
}

bool Application::OnUserDestroy()
{
    return true;
}

bool Application::OnUserRender()
{
    return true;
}

bool Application::init(const std::string title, int width, int height, float scale, const bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_scale = scale;
    m_title = title;
    m_fullscreen = fullscreen;

    if(m_fullscreen)
    {
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
    }
    else
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    InitWindow(m_width * m_scale, m_height * m_scale, m_title.c_str());
    SetWindowMinSize(m_width, m_height);
    m_render_texture = LoadRenderTexture(m_width, m_height);
    SetTextureFilter(m_render_texture.texture, FILTER_POINT);

    SetTargetFPS(60);

    // Compute required framebuffer scaling
    if(m_fullscreen)
    {
        m_framebuffer_scale = min((float)GetMonitorWidth(0) / m_width, (float)GetMonitorHeight(0) / m_height);
    }
    else
    {
        m_framebuffer_scale = min((float)GetScreenWidth() / m_width, (float)GetScreenHeight() / m_height);
    }

    m_font = LoadFont("assets/fonts/MedievalSharp-Bold.ttf");

    TraceLog(LOG_INFO,"PixelWolf initialized.");

    return true;
}

void Application::run()
{
    m_running = true;

    if (!OnUserCreate()) m_running = false;

    while (!WindowShouldClose() && m_running)
    {
        // Compute required framebuffer scaling
        if(m_fullscreen)
        {
            m_framebuffer_scale = min((float)GetMonitorWidth(0) / m_width, (float)GetMonitorHeight(0) / m_height);
        }
        else
        {
            m_framebuffer_scale = min((float)GetScreenWidth() / m_width, (float)GetScreenHeight() / m_height);
        }

        double elapsedTime = 0.1;

        event();

        update(elapsedTime);

        OnUserUpdate(elapsedTime);

        render();
    }
    OnUserDestroy();

    UnloadRenderTexture(m_render_texture);
    UnloadFont(m_font);

    CloseWindow();
}

void Application::event()
{
#if defined(_WIN32)
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
    {
        m_fullscreen = !m_fullscreen;
        toggle_fullscreen();
    }
#endif
    // // Update virtual mouse (clamped mouse value behind game screen)
    // m_mouse_position = GetMousePosition();
    // if(m_fullscreen)
    // {
    //     m_virtual_mouse_position.x = (m_mouse_position.x - (GetMonitorWidth(0) - (m_width * m_framebuffer_scale)) * 0.5f) / m_framebuffer_scale;
    //     m_virtual_mouse_position.y = (m_mouse_position.y - (GetMonitorHeight(0) - (m_height * m_framebuffer_scale)) * 0.5f) / m_framebuffer_scale;
    // }
    // else
    // {
    //     m_virtual_mouse_position.x = (m_mouse_position.x - (GetScreenWidth() - (m_width * m_framebuffer_scale)) * 0.5f) / m_framebuffer_scale;
    //     m_virtual_mouse_position.y = (m_mouse_position.y - (GetScreenHeight() - (m_height * m_framebuffer_scale)) * 0.5f) / m_framebuffer_scale;
    // }

    // m_virtual_mouse_position = ClampValue(m_virtual_mouse_position, { 0, 0 }, { static_cast<float>(m_width), static_cast<float>(m_height) }); 

    // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
    //SetMouseOffset(-(GetScreenWidth() - (m_width*m_framebuffer_scale))*0.5f, -(GetScreenHeight() - (m_height*m_framebuffer_scale))*0.5f);
    //SetMouseScale(1/m_framebuffer_scale, 1/m_framebuffer_scale);
    //----------------------------------------------------------------------------------
}

void Application::toggle_fullscreen()
{
    UnloadRenderTexture(m_render_texture);
    UnloadFont(m_font);
    CloseWindow();

    if(m_fullscreen)
    {
        TraceLog(LOG_INFO,"[toggle_fullscreen] Recreating fullscreen window.");
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(m_width * m_scale, m_height * m_scale, m_title.c_str());
        SetWindowMinSize(GetMonitorWidth(0), GetMonitorHeight(0));
    }
    else
    {
        TraceLog(LOG_INFO,"[toggle_fullscreen] Recreating normal window.");
        ClearWindowState(FLAG_FULLSCREEN_MODE);
        SetConfigFlags(FLAG_VSYNC_HINT);
        InitWindow(m_width * m_scale, m_height * m_scale, m_title.c_str());
        SetWindowMinSize(m_width, m_height);
    }

    m_render_texture = LoadRenderTexture(m_width, m_height);
    SetTextureFilter(m_render_texture.texture, FILTER_POINT);
    m_font = LoadFont("assets/fonts/MedievalSharp-Bold.ttf");
}

void Application::update(double elapsedTime)
{
}

void Application::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    OnUserRender();

    BeginTextureMode(m_render_texture);

    ClearBackground(RAYWHITE);

    for(int x = 0; x < GetScreenWidth(); ++x)
    {
        for(int y = 0; y < GetScreenHeight(); ++y)
        {
            Color color = { static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)) };
            DrawPixel(x, y, color);
        }
    }

    DrawFPS(10, 10);

    DrawTextEx(m_font, "PixelWolf", { 50, 50 }, 20, 0, RAYWHITE);
    // DrawText(TextFormat("Default Mouse: [%i , %i]", (int)m_mouse_position.x, (int)m_mouse_position.y), 50, 120, 20, GREEN);
    // DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)m_virtual_mouse_position.x, (int)m_virtual_mouse_position.y), 50, 150, 20, YELLOW);

    EndTextureMode();

    int screen_width = GetScreenWidth();
    int screen_height = GetScreenHeight();
    if(m_fullscreen) {
        screen_width = GetMonitorWidth(0);
        screen_height = GetMonitorHeight(0);
    }
    // Draw RenderTexture2D to window, properly scaled
    DrawTexturePro(m_render_texture.texture, { 0.0f, 0.0f, (float)m_render_texture.texture.width, (float)-m_render_texture.texture.height },
        { (screen_width - ((float)m_width * m_framebuffer_scale)) * 0.5f, (screen_height - ((float)m_height * m_framebuffer_scale)) * 0.5f,
        (float)m_width * m_framebuffer_scale, (float)m_height * m_framebuffer_scale }, { 0, 0 }, 0.0f, WHITE);

    EndDrawing();
}
