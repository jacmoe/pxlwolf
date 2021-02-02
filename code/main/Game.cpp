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
#include "Game.hpp"

Game::Game()
{
    m_map = std::make_shared<utility::Map>();
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    utility::Map* map = m_map.get();
    map->init("assets/levels/pxlwolf.ldtk");
    map->load("Level4");
    return true;
}

bool Game::OnUserUpdate(double elapsedTime)
{
    return true;
}

bool Game::OnUserRender()
{
    for(int x = 0; x < GetScreenWidth(); ++x)
    {
        for(int y = 0; y < GetScreenHeight(); ++y)
        {
            Color color = { static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)) };
            ImageDrawPixel(&m_draw_buffer, x, y, color);
        }
    }

    // DrawText(TextFormat("Default Mouse: [%i , %i]", (int)m_mouse_position.x, (int)m_mouse_position.y), 50, 120, 20, GREEN);
    // DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)m_virtual_mouse_position.x, (int)m_virtual_mouse_position.y), 50, 150, 20, YELLOW);

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
