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

    utility::ImageAtlas atlas;
    atlas.load("assets/textures/spritesheet.png", {512,512});

    write_text("PixelWolf");

    return true;
}

bool Game::OnUserUpdate(double fDeltaTime)
{
    // m_pixelator.get()->randomize();
    m_pixelator.get()->fill({255,255,200,255});
    m_pixelator.get()->drawLine({10,50}, {100, 100}, {224, 13, 170, 255});
    m_pixelator.get()->drawCircle({50,50}, 30, {255,0,0,255});
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
