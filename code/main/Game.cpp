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
#include "raymath.h"

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
    m_camera.h = 0;

    m_camera.dirX = cos(m_camera.angle);
    m_camera.dirY = sin(m_camera.angle);
    Vector2 vect = Vector2Rotate({(float)cos(m_camera.angle), (float)sin(m_camera.angle)}, -90);
    m_camera.planeX = vect.x;
    m_camera.planeY = vect.y * 0.66;

    m_raycaster.init(m_width, m_height, m_map, m_pixelator);

    pixelator->addBuffer("minimap", m_map.get()->width() * 2, m_map.get()->height() * 2);

    return true;
}

bool Game::OnUserUpdate(double elapsedTime)
{
    utility::Map* map = m_map.get();

    double moveSpeed = elapsedTime * 3.0; //the constant value is in squares/second
    double rotSpeed = elapsedTime * 2.0; //the constant value is in radians/second
    if (IsKeyDown(KEY_W))
    {
        if(map->get_wall_entry(int(m_camera.x + m_camera.dirX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x += m_camera.dirX * moveSpeed;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y + m_camera.dirY * moveSpeed)) < 1)
        {
            m_camera.y += m_camera.dirY * moveSpeed;
        }
    }
    if (IsKeyDown(KEY_S))
    {
        if(map->get_wall_entry(int(m_camera.x - m_camera.dirX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x -= m_camera.dirX * moveSpeed;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y - m_camera.dirY * moveSpeed)) < 1)
        {
            m_camera.y -= m_camera.dirY * moveSpeed;
        }
    }
    if (IsKeyDown(KEY_D))
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = m_camera.dirX;
        m_camera.dirX = m_camera.dirX * cos(-rotSpeed) - m_camera.dirY * sin(-rotSpeed);
        m_camera.dirY = oldDirX * sin(-rotSpeed) + m_camera.dirY * cos(-rotSpeed);
        double oldPlaneX = m_camera.planeX;
        m_camera.planeX = m_camera.planeX * cos(-rotSpeed) - m_camera.planeY * sin(-rotSpeed);
        m_camera.planeY = oldPlaneX * sin(-rotSpeed) + m_camera.planeY * cos(-rotSpeed);
    }
    if (IsKeyDown(KEY_A))
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = m_camera.dirX;
        m_camera.dirX = m_camera.dirX * cos(rotSpeed) - m_camera.dirY * sin(rotSpeed);
        m_camera.dirY = oldDirX * sin(rotSpeed) + m_camera.dirY * cos(rotSpeed);
        double oldPlaneX = m_camera.planeX;
        m_camera.planeX = m_camera.planeX * cos(rotSpeed) - m_camera.planeY * sin(rotSpeed);
        m_camera.planeY = oldPlaneX * sin(rotSpeed) + m_camera.planeY * cos(rotSpeed);
    }

    m_raycaster.raycast(m_camera);

    if(m_show_map)
    {
        m_raycaster.drawMinimap("minimap", m_camera, 2);
    }

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
