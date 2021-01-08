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
#include "Map.hpp"
#include <SDL.h>

Game::Game()
{}

Game::~Game()
{}

// Called once on application startup, use to load your resources
bool Game::OnUserCreate()
{
    Map map;
    map.load("assets/levels/levels.ldtk", "Level1");
    write_text("PixelWolf");
    return true;
}
// Called every frame, and provides you with a time per frame value
bool Game::OnUserUpdate(double fDeltaTime)
{
    return true;
}

bool Game::OnUserRender()
{
	SDL_Rect dst;
	dst.x = 10;
	dst.y = 10;
    SDL_QueryTexture(m_font_texture.get(), NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(m_renderer.get(), m_font_texture.get(), nullptr, &dst);
    return true;
}

// Called once on application termination, so you can be one clean coder
bool Game::OnUserDestroy()
{
    return true;
}
