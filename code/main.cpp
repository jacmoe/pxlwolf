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

#include <iostream>
#include <fstream>
#include <filesystem>

#define DG_MISC_IMPLEMENTATION
#include "DG_misc.hpp"
#include "toml.hpp"
#include "utils.hpp"
#include "Game.hpp"

void setup_working_directory()
{
    // Get executable path
	std::string path = DG_GetExecutableDir();
    // Remove the build directory, so that we land on appropriate directory for asset loading
	std::vector<std::string> strList;
    strList.push_back("/build/code/");
    strList.push_back("\\build\\code\\");
    strList.push_back("\\vsbuild\\code\\");
    strList.push_back("Release");
    strList.push_back("Debug");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
	std::filesystem::current_path(path);
}

int main()
{
    setup_working_directory();

    auto config = toml::parse("assets/config/pxlwolf.toml");
    const auto& application_config = toml::find(config, "application");
    Game game;

    if(game.init(toml::find<std::string>(application_config, "title"),
         toml::find<int>(application_config, "window_width"),
         toml::find<int>(application_config, "window_height"),
         toml::find<float>(application_config, "window_scale"),
         toml::find<bool>(application_config, "fullscreen")))
    {
        game.run();
    }

    return 0;
}
