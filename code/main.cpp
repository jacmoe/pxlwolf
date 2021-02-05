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
#include <memory>
#include <ctime>
#include <cstdio>

#include "toml.hpp"
#include "utils.hpp"
#include "physfs.hpp"


#include "SDL2/SDL.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

void setup_working_directory()
{
    // Get executable path
    std::string path = std::filesystem::current_path().generic_string();
    // Remove the build directory, so that we land on appropriate directory for asset loading
    std::vector<std::string> strList;
    strList.push_back("/build/code");
    strList.push_back("/vsbuild/code");
    strList.push_back("Release");
    strList.push_back("RelWidthDebInfo");
    strList.push_back("Debug");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
    std::filesystem::current_path(path);
}

void setup_logging()
{
    std::shared_ptr<spdlog::logger> m_pxllogger;

    std::string logfile_name = "log/pxllog.txt";
    
    // Remove old log file
    if(std::filesystem::exists(logfile_name))
    {
        std::remove(logfile_name.c_str());
    }

	// Create console sink and file sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile_name, true);
	spdlog::sinks_init_list sink_list = { file_sink, console_sink };
	// Make the logger use both the console and the file sink
    m_pxllogger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
	// Set the standard logger so that we can use it freely everywhere
    spdlog::set_default_logger(m_pxllogger);
	// Set the format pattern - [Loglevel] [Function] [Line] message
	spdlog::set_pattern("[%l] [%!] [line %#] %v");
}

int main(int, char**)
{
    setup_working_directory();
    setup_logging();

    // Remove old log file
    if(std::filesystem::exists("log/pxllog.txt"))
    {
        std::remove("log/pxllog.txt");
    }

	SPDLOG_INFO("PixelWolf initializing.");

    auto config = toml::parse("assets/config/pxlwolf.toml");
    const auto& application_config = toml::find(config, "application");
    toml::table config_table = toml::get<toml::table>(application_config);

#if defined(_MSC_VER)
# pragma warning(push)
// Disable TOML conversion warnings
# pragma warning(disable: 4244)
#endif
    const int screenWidth = config_table["window_width"].as_integer();
    const int screenHeight = config_table["window_height"].as_integer();
    const float scale = config_table["window_scale"].as_integer();
    const std::string title = config_table["title"].as_string();
    const bool fullscreen = config_table["fullscreen"].as_boolean();
#if defined(_MSC_VER)
# pragma warning(pop)
#endif

    // Game game;

    // if(game.init(title, screenWidth, screenHeight, scale, fullscreen))
    // {
    //     game.run();
    // }

   if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        
        SDL_CreateWindow(
            "SDL2 Demo",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            360, 240,
            SDL_WINDOW_SHOWN
        );
        
        SDL_Delay(2000);
    }
 	SPDLOG_INFO("PixelWolf exited.");

    return 0;
}
