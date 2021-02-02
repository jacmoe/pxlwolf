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
    map->load("Level4");

    Pixelator* pixelator = m_pixelator.get();

    if(m_map->loaded())
    {
        m_camera.x = m_map.get()->player_start().x;
        m_camera.y = m_map.get()->player_start().y;
        m_camera.angle = m_map.get()->player_heading();
        m_camera.dist = 36;
        m_camera.fov = PI / 2;
        m_camera.h = 0;

        m_raycaster.init(m_width, m_height, m_map, m_pixelator);

        m_raycaster.generateAngleValues(m_width, &m_camera);

        m_raycaster.resetDepthBuffer();

        m_raycaster.raycastRender(&m_camera, 0.01);

        m_raycaster.renderBuffer();

        pixelator->addBuffer("minimap", m_map.get()->width() * 2, m_map.get()->height() * 2);
        // pixelator->addBuffer("minimap", m_width, m_height);

        m_raycaster.drawMinimap("minimap", m_camera, 2);

        pixelator->copy("pixelBuffer");

        pixelator->copy("minimap", 200, 0);
    }

    return true;
}

bool Game::OnUserUpdate(double elapsedTime)
{
    return true;
}

bool Game::OnUserRender()
{

    // DrawText(TextFormat("Default Mouse: [%i , %i]", (int)m_mouse_position.x, (int)m_mouse_position.y), 50, 120, 20, GREEN);
    // DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)m_virtual_mouse_position.x, (int)m_virtual_mouse_position.y), 50, 150, 20, YELLOW);

    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
