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
#include "main/Application.hpp"

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
    , font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , font_size(12)
    , m_screenlock(nullptr)
{}

Application::~Application()
{
    SPDLOG_INFO("PixelWolf shutdown.");
}

bool Application::init()
{

    if (!al_init())
    {
        SPDLOG_ERROR("Couldn't initialize allegro");
        return false;
    }

    SPDLOG_INFO("Allegro {} initialized.", ALLEGRO_VERSION_STR);

    m_config.reset(al_load_config_file("assets/config/pxlwolf.ini"));
    if (!m_config.get())
    {
        SPDLOG_ERROR("Couldn't load configuration");
        return false;
    }

    m_width = std::stoi(al_get_config_value(m_config.get(), "graphics", "width"));
    m_height = std::stoi(al_get_config_value(m_config.get(), "graphics", "height"));
    m_scale = std::stof(al_get_config_value(m_config.get(), "graphics", "scale"));
    m_title = al_get_config_value(m_config.get(), "graphics", "title");
    int fscr = std::stoi(al_get_config_value(m_config.get(), "graphics", "fullscreen"));
    m_fullscreen = (fscr > 0);

    if (!al_install_keyboard())
    {
        SPDLOG_ERROR("Couldn't initialize keyboard");
        return false;
    }

    m_timer.reset(al_create_timer(1.0 / 60));
    if (!m_timer.get())
    {
        SPDLOG_ERROR("Couldn't initialize timer");
        return false;
    }

    m_queue.reset(al_create_event_queue());
    if (!m_queue.get())
    {
        SPDLOG_ERROR("Couldn't initialize queue");
        return false;
    }

    m_display.reset(al_create_display(m_width * m_scale, m_height * m_scale));
    if (!m_display.get())
    {
        SPDLOG_ERROR("Couldn't initialize display");
        return false;
    }

    m_font.reset(al_create_builtin_font());
    if (!m_font.get())
    {
        SPDLOG_ERROR("Couldn't initialize font");
        return false;
    }

    if (!al_init_image_addon())
    {
        SPDLOG_ERROR("Couldn't initialize image addon");
        return false;
    }

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_NO_PRESERVE_TEXTURE);

    int flags = al_get_new_bitmap_flags();

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    m_display_buffer.reset(al_create_bitmap(m_width, m_height));
    if (!m_display_buffer.get())
    {
        SPDLOG_ERROR("Couldn't create display buffer");
        return false;
    }

    al_set_new_bitmap_flags(flags);

    al_register_event_source(m_queue.get(), al_get_keyboard_event_source());
    al_register_event_source(m_queue.get(), al_get_display_event_source(m_display.get()));
    al_register_event_source(m_queue.get(), al_get_timer_event_source(m_timer.get()));

    m_pixelator = std::make_shared<Pixelator>();

    m_pixelator.get()->setSize(Vector2i(m_width, m_height));

    SPDLOG_INFO("PixelWolf initialized.");
    return true;
}

void Application::run()
{
    bool done = false;
    bool redraw = true;
    double old_time = al_get_time();
    double time_now = 0.0;
    double delta_time = 0.0;

    OnUserCreate();
    
    al_start_timer(m_timer.get());

    while (1)
    {
        al_wait_for_event(m_queue.get(), &m_event);


        switch (m_event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            m_average_fps = 1.0 / (al_get_time() - old_time);
            time_now = al_get_time();
            delta_time = time_now - old_time;
            OnUserUpdate(delta_time);
            old_time = time_now;
            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:

            al_get_keyboard_state(&m_keyboard_state);
            if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_ESCAPE))
            {
                done = true;
            }
            if (al_key_down(&m_keyboard_state, ALLEGRO_KEY_P))
            {
                save_screenshot();
            }
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (redraw && al_is_event_queue_empty(m_queue.get()))
        {
            render();
            redraw = false;
        }
    }
}

void Application::save_screenshot()
{
    al_save_bitmap("screenshot.png", al_get_backbuffer(m_display.get()));
}

void Application::event()
{
}

void Application::update_display_buffer()
{
    m_screenlock = al_lock_bitmap(m_display_buffer.get(), ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);

    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            ALLEGRO_COLOR color = m_pixelator.get()->getPixel(x, y);
            al_put_pixel(x, y, color);
        }
    }

    al_unlock_bitmap(m_display_buffer.get());
}

void Application::render()
{
    al_set_target_bitmap(m_display_buffer.get());
    al_clear_to_color(al_map_rgb(0, 0, 0));

    update_display_buffer();

    al_set_target_backbuffer(m_display.get());
    al_draw_scaled_bitmap(m_display_buffer.get(), 0, 0, m_width, m_height, 0, 0, m_width * m_scale, m_height * m_scale, 0);

    OnUserRender();

    al_flip_display();
}
