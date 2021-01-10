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
{}

Game::~Game()
{}


bool Game::OnUserCreate()
{
    buffer = Pixelator::CreatePixelBuffer(800, 600, sf::Color::Red);

    texture.create(m_width, m_height);

    texture.update(&buffer->pixels[0]);

    sprite.setTexture(texture);
    sprite.setScale(m_scale, m_scale);

    return true;
}

bool Game::OnUserUpdate(double fDeltaTime)
{
    return true;
}

bool Game::OnUserRender()
{
    m_renderwindow.get()->draw(sprite);
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
