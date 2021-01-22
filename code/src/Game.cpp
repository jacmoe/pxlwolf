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

    m_pixelator.addBuffer("secondary");

    m_pixelator.setActiveBuffer("secondary");

    m_pixelator.copy(atlas.GetPixels(7), atlas.GetImageDimensions(), 0, 0, sf::IntRect(0, 0, atlas.GetImageDimensions().x, atlas.GetImageDimensions().y));

    m_pixelator.setActiveBuffer("primary");

    m_pixelator.drawColumn(100, 2, 50, sf::Color::Blue);

    m_pixelator.drawRow(10, 10, 200, sf::Color::Red);

    m_pixelator.drawFilledRect(sf::IntRect(10, 10, 10, 10), sf::Color::Cyan);

    m_pixelator.drawLine(sf::Vector2i(10,10), sf::Vector2i(200, 80), sf::Color::Magenta);

    m_pixelator.drawCircle(sf::Vector2i(100,100), 80, sf::Color::Blue);

    m_pixelator.drawRect(sf::IntRect(60, 60, 100, 100), sf::Color::White);

    m_pixelator.setActiveBuffer("secondary");

    m_pixelator.copy("primary", 0, 0, true);

    m_action_map["test"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);

    //void RaycasterEngine::drawMinimap(PixBuffer* buffer, Camera* camera, unsigned int width, unsigned int height, Map* map, int blockSize)
    int height = m_height;
    int width = m_width;
    int blockSize = 2;
    int row, col;
    float mapGridSquareSize = (float)height / (float)blockSize;
    int mapXOffset = (width - height) / 2;
    int mapYOffset = (height - height) / 2;
    sf::IntRect mapRect;
    mapRect.width = map.width() * blockSize;
    mapRect.height = map.height() * blockSize;
    mapRect.left = width - mapRect.width;
    mapRect.top = 0;
    sf::IntRect blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;

    int p_x = map.player_position().x;
    int p_y = map.player_position().y;

    /* Draw map tiles */
    for(row = 0; row < map.height(); row++)
    {
        for(col = 0; col < map.width(); col++)
        {
            blockRect.left = mapRect.left + col * blockSize;
            blockRect.top = mapRect.top + row * blockSize;
            if(map.get_walls()[row * map.width() + col] > 0)
            {
                sf::Color blockcolor = map.get_wall_element(map.get_walls()[row * map.width() + col]).color;
                m_pixelator.drawFilledRect(blockRect, blockcolor);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                sf::Color sepiaPink = {221,153,153,255};
                m_pixelator.drawFilledRect(blockRect, sepiaPink);
            }
        }
    }



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
