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
#include "main/Game.hpp"
#include <allegro5/allegro_color.h>

Game::Game()
    : m_delta_time(0.0)
{
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    Pixelator* pixelator = m_pixelator.get();

    pixelator->fill(al_color_name("darkgreen"));

    pixelator->drawCircle(Vector2i(30, 30), 20, al_color_name("goldenrod"));

    return true;
}

bool Game::OnUserUpdate(double deltaTime)
{
    m_delta_time = deltaTime;
    m_delta_time *= 1000000;
    return true;
}

bool Game::OnUserRender()
{
    al_draw_textf(m_font.get(), al_map_rgb(255, 255, 255), 10.0, 10.0, 0, "Deltatime : %.12f", m_delta_time);

    m_pixelator.get()->blendAlpha(al_color_name("darkred"), 0.04);

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
