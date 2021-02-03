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

Game::Game()
: m_raycaster()
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
    map->load("Level5");

    Pixelator* pixelator = m_pixelator.get();

    m_camera.x = m_map.get()->player_start().x;
    m_camera.y = m_map.get()->player_start().y;
    m_camera.angle = m_map.get()->player_heading();
    m_camera.dist = 36;
    m_camera.fov = PI / 2;
    m_camera.h = 0;

    m_raycaster.init(m_width, m_height, m_map, m_pixelator);

    m_raycaster.generateAngleValues(m_width, &m_camera);

    pixelator->addBuffer("minimap", m_map.get()->width() * 2, m_map.get()->height() * 2);

    return true;
}

bool Game::OnUserUpdate(double elapsedTime)
{
    m_pixelator->fill(BLACK);
    if (IsKeyDown(KEY_D))
    {
        m_camera.angle += 0.8 * elapsedTime;
    }
    if (IsKeyDown(KEY_A))
    {
        m_camera.angle -= 0.8 * elapsedTime;
    }

    m_raycaster.resetDepthBuffer();

    m_raycaster.raycastRender(&m_camera, 0.01);

    m_raycaster.renderBuffer();

    if(m_show_map)
    {
        m_raycaster.drawMinimap("minimap", m_camera, 2);
    }

    m_pixelator.get()->copy("pixelBuffer");

    if(m_show_map)
    {
        m_pixelator.get()->copy("minimap", 200, 100);
    }

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
