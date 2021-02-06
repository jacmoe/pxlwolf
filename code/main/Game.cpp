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
    map->load("Level1");

    write_text("PixelWolf");

    m_camera.x = m_map.get()->player_start().x;
    m_camera.y = m_map.get()->player_start().y;
    m_camera.angle = m_map.get()->player_heading();
    m_camera.h = 0;

    m_camera.dirX = cos(m_camera.angle);
    m_camera.dirY = sin(m_camera.angle);
    double v_y = std::sin(m_camera.angle);
    double v_x = std::cos(m_camera.angle);
    linalg::aliases::double2 vect = {v_x, v_y};
    double a_y = std::sin(-90 * 4.0 * atan (1.0) / 180.0);
    double a_x = std::cos(-90 * 4.0 * atan (1.0) / 180.0);
    linalg::aliases::double2 vect_rotated =  {vect.x*a_x - vect.y*a_y, vect.x*a_y + vect.y*a_x};
    m_camera.planeX = vect_rotated.x;
    m_camera.planeY = vect_rotated.y * 0.66;
    m_camera.pitch = 0; // looking up/down, expressed in screen pixels the horizon shifts
    m_camera.z = 0; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts

    m_raycaster.init(m_width, m_height, m_map, m_pixelator);

    m_pixelator.get()->addBuffer("minimap", m_map.get()->width() * 2, m_map.get()->height() * 2);

    return true;
}

bool Game::OnUserUpdate(double fDeltaTime)
{
    m_raycaster.raycastCeilingFloor(m_camera);
    m_raycaster.raycast(m_camera);
    m_raycaster.drawMinimap("primary", m_camera, 2);
    return true;
}

bool Game::OnUserRender()
{
    SDL_UpdateTexture(m_render_texture.get(), NULL, m_pixelator.get()->getPixels(), sizeof(SDL_Color) * m_width);
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
