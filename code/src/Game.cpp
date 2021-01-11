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
{}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    ImageAtlas atlas;

    atlas.Init("assets/textures/walls.png", 10, 1);

    //buffer = Pixelator::CreatePixelBuffer(m_width, m_height, commodoreColorPallette[4]);

    sf::Image dest_image;
    dest_image.create(m_width, m_height);

    int x = 0;
    for (int i = 0; i < atlas.GetRows(); i++)
    {
        for (int j = 0; j < atlas.GetCols(); j++)
        {
            sf::Image image;
            image.create(atlas.GetImageDimensions().x, atlas.GetImageDimensions().y, atlas.GetPixels(x));
            x++;
            dest_image.copy(image, j * atlas.GetImageDimensions().x, i * atlas.GetImageDimensions().y);
        }
    }

    m_rendertexture.update(dest_image);

    //m_rendertexture.update(pixels);

    write_text("Hello from PixelWolf!");

    return true;
}

bool Game::OnUserUpdate(sf::Time elapsedTime)
{
    write_text("Hello from PixelWolf! Time is : " + std::to_string(elapsedTime.asSeconds()) + " Frames per second : " + std::to_string(m_frames_per_second));
    return true;
}

bool Game::OnUserRender()
{
    return true;
}

bool Game::OnUserDestroy()
{
    //delete buffer;
    return true;
}
