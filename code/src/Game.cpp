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

    atlas.Init("assets/textures/sjswalls2.bmp", 4, 3);

    sf::Image image;
    image.create(atlas.GetImageDimensions().x, atlas.GetImageDimensions().y, atlas.GetPixels(7));

    for(unsigned int px = 0; px < image.getSize().x; px++ )
    {
        for(unsigned int py = 0; py < image.getSize().y; py++ )
        {
            m_pixelator.setPixel(px, py, image.getPixel(px, py));
        }
    }

    //m_pixelator.fill(sf::Color::Red);

    // int bufnum = m_pixelator.copy();

    SPDLOG_INFO("Pixelators current buffer index is '{}'", m_pixelator.getActiveBuffer());

    unsigned int bufnum = m_pixelator.addBuffer("test_buffer");

    SPDLOG_INFO("Added a Pixelator buffer with name '{}'", bufnum);

    SPDLOG_INFO("Pixelator has {} buffers", m_pixelator.getNumberOfBuffers());

    SPDLOG_INFO("Pixelators current buffer index is '{}'", m_pixelator.getActiveBuffer());

    m_pixelator.removeBuffer("test_buffer");

    SPDLOG_INFO("Pixelator has {} buffers", m_pixelator.getNumberOfBuffers());

    SPDLOG_INFO("Trying to remove active buffer . . .");
    m_pixelator.removeBuffer("primary");

    m_pixelator.addBuffer("second_buffer");
    m_pixelator.setActiveBuffer("second_buffer");
    m_pixelator.fill(sf::Color::Blue);

    m_pixelator.setActiveBuffer("primaryy");

    SPDLOG_INFO("Pixelator has {} buffers", m_pixelator.getNumberOfBuffers());

    SPDLOG_INFO("Pixelators current buffer is '{}'", m_pixelator.getActiveBuffer());

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
    return true;
}
