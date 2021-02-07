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

void Game::adjustCameraVectors()
{
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
}

bool Game::OnUserCreate()
{
    utility::Map* map = m_map.get();
    map->init("assets/levels/pxlwolf.ldtk");
    map->load("Level5");

    write_text("PixelWolf");

    m_camera.x = m_map.get()->player_start().x;
    m_camera.y = m_map.get()->player_start().y;
    m_camera.angle = m_map.get()->player_heading();
    m_camera.h = 0;

    adjustCameraVectors();

    m_camera.pitch = 0; // looking up/down, expressed in screen pixels the horizon shifts
    m_camera.z = 0; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts

    m_raycaster.init(m_width, m_height, m_map, m_pixelator);

    m_pixelator.get()->addBuffer("minimap", m_map.get()->width() * 2, m_map.get()->height() * 2);

    return true;
}

bool Game::OnUserUpdate(double deltaTime)
{
    double delta_seconds = deltaTime * 0.001;

    write_text("PixelWolf - " + std::to_string( get_fps()) + " FPS. Deltatime : " + std::to_string(delta_seconds));

    utility::Map* map = m_map.get();
    m_raycaster.raycastCeilingFloor(m_camera);
    m_raycaster.raycast(m_camera);
    if(m_show_map)
    {
        m_raycaster.drawMinimap("primary", m_camera, 2);
    }

    double moveSpeed = delta_seconds * 3.0; //the constant value is in squares/second
    double rotSpeed = delta_seconds * 2.0; //the constant value is in radians/second

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if( currentKeyStates[ SDL_SCANCODE_LSHIFT ] ) // run
    {
        moveSpeed *= 3.0;
    }

    if( currentKeyStates[ SDL_SCANCODE_M ] ) // toggle minimap
    {
        m_show_map = !m_show_map;
    }

    if( currentKeyStates[ SDL_SCANCODE_R ] ) // respawn player at player start
    {
        m_camera.x = m_map.get()->player_start().x;
        m_camera.y = m_map.get()->player_start().y;
        m_camera.angle = m_map.get()->player_heading();
        adjustCameraVectors();
    }

    if( currentKeyStates[ SDL_SCANCODE_W ] ) // move forward
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
    if( currentKeyStates[ SDL_SCANCODE_S ] ) // move backward
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
    if( currentKeyStates[ SDL_SCANCODE_D ] ) // turn right
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = m_camera.dirX;
        m_camera.dirX = m_camera.dirX * cos(-rotSpeed) - m_camera.dirY * sin(-rotSpeed);
        m_camera.dirY = oldDirX * sin(-rotSpeed) + m_camera.dirY * cos(-rotSpeed);
        double oldPlaneX = m_camera.planeX;
        m_camera.planeX = m_camera.planeX * cos(-rotSpeed) - m_camera.planeY * sin(-rotSpeed);
        m_camera.planeY = oldPlaneX * sin(-rotSpeed) + m_camera.planeY * cos(-rotSpeed);
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] ) // turn left
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = m_camera.dirX;
        m_camera.dirX = m_camera.dirX * cos(rotSpeed) - m_camera.dirY * sin(rotSpeed);
        m_camera.dirY = oldDirX * sin(rotSpeed) + m_camera.dirY * cos(rotSpeed);
        double oldPlaneX = m_camera.planeX;
        m_camera.planeX = m_camera.planeX * cos(rotSpeed) - m_camera.planeY * sin(rotSpeed);
        m_camera.planeY = oldPlaneX * sin(rotSpeed) + m_camera.planeY * cos(rotSpeed);
    }

    if (SDL_GetRelativeMouseMode())
    {
        // Get mouse change since last frame
        int32_t mouseX;
        SDL_GetRelativeMouseState(&mouseX, NULL);
        // Update player
        m_camera.angle -= 0.1 * rotSpeed * mouseX;
        adjustCameraVectors();
    }

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
