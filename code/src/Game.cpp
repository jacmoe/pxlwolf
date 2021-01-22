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
#include "lua_main.hpp"
#include "physfs.hpp"
#include "RayCaster.hpp"

Game::Game()
{
    m_map = std::make_shared<Map>();
    m_atlas = std::make_shared<ImageAtlas>();
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    ImageAtlas* atlas = m_atlas.get();
    Pixelator* pixelator = m_pixelator.get();

    m_map.get()->load("assets/levels/levels.ldtk", "Level1");

    atlas->Init("assets/textures/sjswalls2.bmp", 4, 3);


    pixelator->addBuffer("secondary");

    pixelator->setActiveBuffer("secondary");

    pixelator->copy(atlas->GetPixels(7), atlas->GetImageDimensions(), 0, 0, sf::IntRect(0, 0, atlas->GetImageDimensions().x, atlas->GetImageDimensions().y));

    pixelator->setActiveBuffer("primary");

    pixelator->drawColumn(100, 2, 50, sf::Color::Blue);

    pixelator->drawRow(10, 10, 200, sf::Color::Red);

    pixelator->drawFilledRect(sf::IntRect(10, 10, 10, 10), sf::Color::Cyan);

    pixelator->drawLine(sf::Vector2i(10,10), sf::Vector2i(200, 80), sf::Color::Magenta);

    pixelator->drawCircle(sf::Vector2i(100,100), 80, sf::Color::Blue);

    pixelator->drawRect(sf::IntRect(60, 60, 100, 100), sf::Color::White);

    pixelator->setActiveBuffer("secondary");

    pixelator->copy("primary", 0, 0, true);

    m_action_map["test"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);

    RayCaster raycaster(m_map, m_pixelator);

    Camera camera;
    raycaster.drawMinimap("secondary", "minimap", camera, 2);

    pixelator->copy("minimap", 200, 0, true);

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
