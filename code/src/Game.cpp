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

Game::Game()
{}

Game::~Game()
{}

// Called once on application startup, use to load your resources
bool Game::OnUserCreate()
{
    SPDLOG_INFO("Creating things");
    Map map;
    map.load("assets/levels/levels.ldtk", "Level1");

    return true;
}
// Called every frame, and provides you with a time per frame value
bool Game::OnUserUpdate(double fDeltaTime)
{
    static bool did_it = false;
    static unsigned short counter = 0;
    if(!did_it)
    {
        SPDLOG_INFO("Updating with fDeltaTime : {}", fDeltaTime);
        counter++;
        if(counter > 10) did_it = true;
    }
    return true;
}
// Called once on application termination, so you can be one clean coder
bool Game::OnUserDestroy()
{
    SPDLOG_INFO("Destroying things");
    return true;
}
