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
#include <set>
#include <fstream>
#include <filesystem>
#include "spdlog/stopwatch.h"

Game::Game()
{}

Game::~Game()
{
}
void removeDuplicates(std::vector<sf::Color>& arr) {
    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = i + 1; j < arr.size(); j++)
        {
            if (arr[i].toInteger() > arr[j].toInteger())
            {
                sf::Color temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    std::vector<sf::Color> y;
    sf::Color x = arr[0];
    int i = 0;
    while (i < arr.size())
    {
        if (x != arr[i])
        {
            y.push_back(x);
            x = arr[i];
        }
        i++;
        if (i == arr.size())
            y.push_back(arr[i - 1]);
    }
    arr = y;
}

int getIndex(std::vector<sf::Color> v, sf::Color color)
{
    auto it = std::find(v.begin(), v.end(), color);
 
    // If element was found
    if (it != v.end()) 
    {
     
        // calculating the index
        // of K
        return it - v.begin();
    }
    else {
        // If the element is not
        // present in the vector
        return -1;
    }
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

    std::stringstream ss;

    PaletteStruct ps;
    ColorStruct cs;

    if(std::filesystem::exists("assets/data/palcol.dat"))
    {
        SPDLOG_INFO("Loading from 'assets/data/palcol.dat'");
        spdlog::stopwatch sw;
        std::ifstream is("assets/data/palcol.dat", std::ios::in | std::ios::binary);

        cereal::BinaryInputArchive iarchive(is);

        iarchive(ps, cs);
        SPDLOG_INFO("Loaded from assets/data/palcol.dat : {:.2}", sw);
    }
    else
    {
        SPDLOG_INFO("Generating 'assets/data/palcol.dat'");
        spdlog::stopwatch sw;
        std::ofstream os("assets/data/palcol.dat", std::ios::out | std::ios::binary);
        cereal::BinaryOutputArchive oarchive(os);

        int idx = 0;
        for(unsigned int px = 0; px < image.getSize().x; px++ )
        {
            for(unsigned int py = 0; py < image.getSize().y; py++ )
            {
                ps.palette_colors.push_back(image.getPixel(px, py));
                idx++;
            }
        }

        SPDLOG_INFO("Size of the_palette before duplicate removal: {}", ps.palette_colors.size());
        removeDuplicates(ps.palette_colors);
        SPDLOG_INFO("Size of the_palette after duplicate removal: {}", ps.palette_colors.size());

        for(unsigned int px = 0; px < image.getSize().x; px++ )
        {
            for(unsigned int py = 0; py < image.getSize().y; py++ )
            {
                cs.colors.push_back(getIndex(ps.palette_colors, image.getPixel(px, py)));
            }
        }

        oarchive(ps, cs);
        SPDLOG_INFO("Created assets/data/test.dat : {:.2}", sw);
    }

    m_pixeldisplay.setPalette(ps.palette_colors);

    int idx = 0;
    for(unsigned int px = 0; px < image.getSize().x; px++ )
    {
        for(unsigned int py = 0; py < image.getSize().y; py++ )
        {
            m_pixeldisplay.setPixel(m_pixeldisplay.getIndex(sf::Vector2u(px, py)), cs.colors[idx]);
            idx++;
        }
    }

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
