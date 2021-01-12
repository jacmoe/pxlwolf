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

    std::vector<sf::Color> the_palette;
    int idx = 0;
    for(unsigned int px = 0; px < image.getSize().x; px++ )
    {
        for(unsigned int py = 0; py < image.getSize().y; py++ )
        {
            the_palette.push_back(image.getPixel(px, py));
            idx++;
        }
    }

    SPDLOG_INFO("Size of the_palette before duplicate removal: {}", the_palette.size());
    removeDuplicates(the_palette);
    SPDLOG_INFO("Size of the_palette after duplicate removal: {}", the_palette.size());

    std::vector<unsigned int> the_colors;
    for(unsigned int px = 0; px < image.getSize().x; px++ )
    {
        for(unsigned int py = 0; py < image.getSize().y; py++ )
        {
            the_colors.push_back(getIndex(the_palette, image.getPixel(px, py)));
        }
    }


    m_pixeldisplay.setPalette(the_palette);

    idx = 0;
    for(unsigned int px = 0; px < image.getSize().x; px++ )
    {
        for(unsigned int py = 0; py < image.getSize().y; py++ )
        {
            m_pixeldisplay.setPixel(m_pixeldisplay.getIndex(sf::Vector2u(px, py)), the_colors[idx]);
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
