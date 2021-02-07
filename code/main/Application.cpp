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

Application::Application()
    : m_scale(0)
    , m_width(0)
    , m_height(0)
    , m_fullscreen(false)
    , m_show_map(false)
    , m_title("")
    , m_running(false)
    , m_show_fps(false)
    , m_should_exit(false)
    , m_renderer(nullptr)
    , font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , font_size(12)
    , font_color({255, 255, 255, 255})
    , m_font(nullptr)
{}

Application::~Application()
{
    SPDLOG_INFO("PixelWolf shutdown.");
    if(m_font != nullptr) TTF_CloseFont(m_font);
    m_font_texture.reset();
    m_render_texture.reset();
    m_renderer.reset();
    TTF_Quit();
    SDL_Quit();
}

bool Application::OnUserCreate()
{
    return true;
}

bool Application::OnUserUpdate(double fDeltaTime)
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

bool Application::write_text(const std::string text)
{
    SDL_Surface* surf = TTF_RenderText_Blended(m_font, text.c_str(), font_color);
    if (surf == nullptr){
        SPDLOG_ERROR("Can't render the font!");
        return false;
    }
    m_font_texture.reset(SDL_CreateTextureFromSurface(m_renderer.get(), surf));
    SDL_FreeSurface(surf);
    return true;
}

bool Application::load_font()
{
    if(std::filesystem::exists(font_name))
    {
        m_font = TTF_OpenFont(font_name.c_str(), font_size);
        if (!m_font){
            SPDLOG_ERROR("Cannot load font!", TTF_GetError());
            return false;
        }	
    }
    else
    {
        SPDLOG_ERROR("Font '{}' could not be found.", font_name);
        return false;
    }
    return true;
}

bool Application::init(const std::string title, int width, int height, float scale, const bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_scale = scale;
    m_title = title;
    m_fullscreen = fullscreen;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        SPDLOG_ERROR("Error initializing SDL: {}", std::string(SDL_GetError()));
        return false;
    }
    if (0 != TTF_Init())
    {
        SPDLOG_ERROR("Error initializing TTF: {}", std::string(TTF_GetError()));
        return false;
    }

    m_window.reset(SDL_CreateWindow(
        title.c_str(),
        300, 100,
        width * scale, height * scale,
        SDL_WINDOW_OPENGL
    ));
    if (!m_window)
    {
        SPDLOG_ERROR("Error creating window: {}", std::string(SDL_GetError()));
        return false;
    }

    m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!m_renderer)
    {
        SPDLOG_ERROR("Error creating renderer: {}", std::string(SDL_GetError()));
        return false;
    }

    SDL_RenderSetScale(m_renderer.get(), static_cast<float>(scale), static_cast<float>(scale));
    SDL_SetRenderDrawBlendMode(m_renderer.get(), SDL_BLENDMODE_BLEND);

    m_render_texture.reset(SDL_CreateTexture(m_renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, m_width, m_height));
    SDL_SetTextureBlendMode(m_render_texture.get(), SDL_BLENDMODE_BLEND);

    m_pixelator = std::make_shared<Pixelator>();

    m_pixelator.get()->setSize(m_width, m_height);

    if(!load_font())
    {
        return false;
    }

    SPDLOG_INFO("PixelWolf initialized.");

    return true;
}

void Application::run()
{
    uint32_t counted_frames = 0;
    m_fps_timer.start();

    m_running = true;

    if (!OnUserCreate()) m_running = false;

    while (m_running)
    {
        m_average_fps = counted_frames / (m_fps_timer.getTicks() / 1000.f);
        if(m_average_fps > 2000000)
        {
            m_average_fps = 0;
        }
        event();

        update(m_average_fps);
        OnUserUpdate(m_average_fps);

        render();
        ++counted_frames;
    }
    m_fps_timer.stop();
    OnUserDestroy();
}

void Application::event()
{
    const Uint8 * keystate = SDL_GetKeyboardState(nullptr);

    while (SDL_PollEvent(&e_))
    {
        switch (e_.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;

            case SDL_KEYDOWN:
                switch (e_.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        m_running = false;
                        break;
                }
        }
    }
}

void Application::toggle_fullscreen()
{
}

void Application::update(double elapsedTime)
{
}

void Application::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 255, 255, 255, 255);
    SDL_RenderClear(m_renderer.get());

    OnUserRender();

    SDL_RenderCopy(m_renderer.get(), m_render_texture.get(), NULL, NULL);
    SDL_Rect dst;
    dst.x = 10;
    dst.y = 10;
    SDL_QueryTexture(m_font_texture.get(), NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(m_renderer.get(), m_font_texture.get(), nullptr, &dst);

    SDL_RenderPresent(m_renderer.get());
}
