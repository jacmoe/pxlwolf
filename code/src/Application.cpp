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
#include "Application.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

#define DG_MISC_IMPLEMENTATION
#include "DG_misc.hpp"
#include "utils.hpp"
#include "dbg_console.hpp"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


Application::Application()
    : running_(false)
    , renderer_(new SDLRenderer)
{}

Application::~Application()
{
    //top_texture_.reset();
    renderer_.reset();
    SDL_Quit();
	closeConsoleWindow();
}

bool Application::OnUserCreate()
{
    return true;
}

bool Application::OnUserUpdate(double fDeltaTime)
{
    return true;
}

bool Application::OnUserDestroy()
{
    return true;
}

void Application::setup_working_directory()
{
    // Get executable path
	std::string path = DG_GetExecutableDir();
    // Remove the build directory, so that we land on appropriate directory for asset loading
	std::vector<std::string> strList;
    strList.push_back("/build/code/");
    strList.push_back("\\build\\code\\");
    strList.push_back("Release");
    strList.push_back("Debug");
    utils::eraseSubStrings(path, strList);
    // Set a proper working directory
	std::filesystem::current_path(path);
}

void Application::setup_logging()
{
    std::string logfile_name = "logs/pxllog.txt";
    
    // Remove old log file
    if(std::filesystem::exists(logfile_name))
    {
        std::remove(logfile_name.c_str());
    }

	// Create console logger and file logger
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile_name, true);
	spdlog::sinks_init_list sink_list = { file_sink, console_sink };
	// Make the logger use both console and file logger
    pxllogger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
	// Set the standard logger so that we can use it freely everywhere
    spdlog::set_default_logger(pxllogger);
	// Set the format pattern - [Loglevel] [Date Time] [File] [Function] [Line] message
	spdlog::set_pattern("[%l] [%D %T] [%s] [%!] [line %#] %v");
}

bool Application::init(const std::string title, const int width, const int height, const int scale)
{
    setup_working_directory();

	CreateConsoleWindow();

    setup_logging();

	SPDLOG_INFO("PixelWolf starting up . . .");

    // Initialize SDL window and renderer
    bool success = renderer_->initialize(
            title,
            width,
            height,
            scale
            );
    if (!success)
    {
        SPDLOG_ERROR("Renderer could not be initialized : {}", renderer_->errorMessage());
        return false;
    }
    return true;
}

void Application::run()
{
	uint8_t frameCounter = 0;
	uint32_t realRunTime = 0;
	double realRunTimeF = 0;
	double dt = 0;
	uint32_t runTime = SDL_GetTicks();
	double runTimeF = (double)runTime/1000;

    running_ = true;

    if (!OnUserCreate()) running_ = false;

    while (running_)
    {
		realRunTime = SDL_GetTicks();		
		realRunTimeF = (double)realRunTime/1000;

        event();

        update();
        OnUserUpdate(realRunTimeF);

        render();
    }
    OnUserDestroy();
}

void Application::event()
{
    const Uint8 * keystate = SDL_GetKeyboardState(nullptr);

    while (SDL_PollEvent(&e_))
    {
        switch (e_.type)
        {
            case SDL_QUIT:
                running_ = false;
                break;

            case SDL_KEYDOWN:
                switch (e_.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running_ = false;
                        break;
                }
        }
    }
}

void Application::update()
{
}

void Application::render()
{
    renderer_->clearScreen();

    renderer_->refreshScreen();
}
