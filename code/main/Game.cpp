/*# This file is part of the
#  █████╗ ██╗     ██╗     ███████╗ ██████╗ ██████╗ ███████╗████████╗████████╗ ██████╗ 
# ██╔══██╗██║     ██║     ██╔════╝██╔════╝ ██╔══██╗██╔════╝╚══██╔══╝╚══██╔══╝██╔═══██╗
# ███████║██║     ██║     █████╗  ██║  ███╗██████╔╝█████╗     ██║      ██║   ██║   ██║
# ██╔══██║██║     ██║     ██╔══╝  ██║   ██║██╔══██╗██╔══╝     ██║      ██║   ██║   ██║
# ██║  ██║███████╗███████╗███████╗╚██████╔╝██║  ██║███████╗   ██║      ██║   ╚██████╔╝
# ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝   ╚═╝      ╚═╝    ╚═════╝ 
#   project
#
#   https://github.com/jacmoe/allegretto
#
#   (c) 2021 Jacob Moena
#
#   MIT License
#*/
#include "main/Game.hpp"
#include <allegro5/allegro_color.h>

Game::Game()
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
    return true;
}

bool Game::OnUserRender()
{
    //al_draw_textf(m_font.get(), al_map_rgb(255, 255, 255), 10.0, 10.0, 0, "FPS : %.2f", m_average_fps / 100000.0);

    m_pixelator.get()->blendAlpha(al_color_name("darkred"), 0.04);

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
