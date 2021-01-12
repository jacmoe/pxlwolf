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
#include "lua_main.hpp"
#include "Map.hpp"
#include "physfs.hpp"

Game::Game()
{}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
	SPDLOG_INFO("Testing Lua . . .");

    sol::state lua{};
    lua.open_libraries(sol::lib::base);

    lua.script("print('Hello World from Lua!')");

	SPDLOG_INFO("Testing Maploading . . .");

	SPDLOG_INFO("Initializing PhysFS.");
	PhysFS::init (nullptr);
	PhysFS::mount("assets.zip", "", 1);

    Map map;
    map.load("assets/levels/levels.ldtk", "Level1", true);

	SPDLOG_INFO("Testing ImageAtlas . . .");

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
	PhysFS::deinit();
    //delete buffer;
    return true;
}
