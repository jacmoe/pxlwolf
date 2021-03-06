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

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include <spdlog/spdlog.h>

#include "lgui/gui.h"
#include "lgui/platform/graphics.h"
#include "lgui/style/defaultstyle.h"
#include "lgui/style/defaultstylecolorscheme.h"

#include "utility/ALDeleter.hpp"
#include "main/Pixelator.hpp"


class Application
{
public:
    Application();
    virtual ~Application();

    bool init();
    void run();

protected:
    virtual bool OnUserCreate() { return true; }
    virtual bool OnUserUpdate(double deltaTime) { return true; }
    virtual bool OnUserRender() { return true; }
    virtual bool OnUserPostRender() { return true; }
    virtual bool OnUserDestroy() { return  true; }
    virtual bool OnUserInput() { return true;  }

    float m_scale;
    int m_width;
    int m_height;
    bool m_fullscreen;
    bool m_show_map;
    bool m_show_fps;
    double m_average_fps;

    std::string m_font_name;
    int m_font_size;
    int m_font_size_title;

    std::unique_ptr<ALLEGRO_CONFIG, utility::ALDeleter> m_config;
    std::unique_ptr<ALLEGRO_TIMER, utility::ALDeleter> m_timer;
    std::unique_ptr<ALLEGRO_EVENT_QUEUE, utility::ALDeleter> m_queue;
    std::unique_ptr<ALLEGRO_DISPLAY, utility::ALDeleter> m_display;
    std::unique_ptr<ALLEGRO_FONT, utility::ALDeleter> m_font;
    std::unique_ptr<ALLEGRO_FONT, utility::ALDeleter> m_title_font;
    std::unique_ptr<ALLEGRO_BITMAP, utility::ALDeleter> m_display_buffer;
    ALLEGRO_LOCKED_REGION* m_screenlock;
    ALLEGRO_KEYBOARD_STATE m_keyboard_state;
    ALLEGRO_MOUSE_STATE m_mouse_state;
    std::shared_ptr<Pixelator> m_pixelator;

    std::unique_ptr<lgui::GUI> m_gui;
    std::unique_ptr<lgui::Graphics> m_gui_graphics;
    lgui::Font m_gui_font;
    lgui::DefaultStyleColorScheme m_color_scheme;
    lgui::DefaultStyle m_gui_default_style;
private:
    std::string m_title;
    bool m_running;
    bool m_should_exit;

    ALLEGRO_EVENT m_event;

    void setup_gui();
    void update_display_buffer();
    void save_screenshot();
    bool process_input();
    void render();
};
