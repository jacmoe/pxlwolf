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

#include <SDL.h>

#define DG_MISC_IMPLEMENTATION
#include "DG_misc.hpp"
#include "utils.hpp"
#include "dbg_console.hpp"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"


Application::Application()
    : running(false)
    , m_renderer(nullptr)
    , font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , font_size(16)
    , font_color({255, 255, 255, 255})
{}

Application::~Application()
{
    TTF_CloseFont(m_font);
    m_font_texture.reset();
    m_renderer.reset();
    TTF_Quit();
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

bool Application::OnUserRender()
{
    return true;
}

bool Application::write_text(const std::string text)
{
	SDL_Surface* surf = TTF_RenderText_Blended(m_font, text.c_str(), font_color);
	if (surf == nullptr){
        SPDLOG_ERROR("Can't render the font!");
		return false;
	}
    m_font_texture.reset(SDL_CreateTextureFromSurface(m_renderer.get(), surf));
	SDL_FreeSurface(surf);
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
    m_pxllogger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list({console_sink, file_sink}));
	// Set the standard logger so that we can use it freely everywhere
    spdlog::set_default_logger(m_pxllogger);
	// Set the format pattern - [Loglevel] [Date Time] [File] [Function] [Line] message
	spdlog::set_pattern("[%l] [%D %T] [%s] [%!] [line %#] %v");
}

void Application::load_font()
{
    m_font = TTF_OpenFont(font_name.c_str(), font_size);
	if (m_font == nullptr){
        SPDLOG_ERROR("Cannot load font!");
	}	
}

bool Application::init(const std::string title, const int width, const int height, const int scale)
{
    setup_working_directory();

	CreateConsoleWindow();

    setup_logging();

    TTF_Init();
    load_font();

	SPDLOG_INFO("PixelWolf starting up . . .");


	m_window.reset(SDL_CreateWindow(
		title.c_str(),
		300, 100,
		width * scale, height * scale,
		SDL_WINDOW_OPENGL
	));

	m_renderer.reset(SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
	SDL_RenderSetScale(m_renderer.get(), scale, scale);
	SDL_SetRenderDrawBlendMode(m_renderer.get(), SDL_BLENDMODE_BLEND);

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

    running = true;

    if (!OnUserCreate()) running = false;

    while (running)
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
                running = false;
                break;

            case SDL_KEYDOWN:
                switch (e_.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
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
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    OnUserRender();

    SDL_RenderPresent(m_renderer.get());
}
