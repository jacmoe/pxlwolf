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
#include "ImageAtlas.hpp"
#include "lua_main.hpp"
#include "Map.hpp"
#include "physfs.hpp"

Game::Game()
{}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    Map map;
    map.load("assets/levels/levels.ldtk", "Level1");

    ImageAtlas atlas;

    atlas.Init("assets/textures/sjswalls2.bmp", 4, 3);

    m_pixelator.copy(atlas.GetPixels(7), atlas.GetImageDimensions(), 0, 0, sf::IntRect(0, 0, atlas.GetImageDimensions().x, atlas.GetImageDimensions().y));

    m_pixelator.drawColumn(100, 2, 50, sf::Color::Blue);

    m_pixelator.drawRow(10, 10, 200, sf::Color::Red);

    m_action_map["test"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);

    return true;
}

bool Game::OnUserUpdate(sf::Time elapsedTime)
{
    write_text("Hello from PixelWolf! Time is : " + std::to_string(elapsedTime.asSeconds()) + " Frames per second : " + std::to_string(m_frames_per_second));

    if (m_action_map.isActive("test"))
        write_text("Left mouse button down!!");

    return true;
}

bool Game::OnUserRender()
{
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
