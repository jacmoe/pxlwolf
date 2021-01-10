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
    : running(false)
    , m_scale(0)
    , m_width(0)
    , m_height(0)
    , m_renderwindow(nullptr)
    , font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , font_size(16)
    , font_color({255, 255, 255, 255})
{}

Application::~Application()
{
    m_renderwindow.reset();
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

bool Application::load_font()
{
    if(std::filesystem::exists(font_name))
    {
    }
    else
    {
        return false;
    }
    return true;
}

bool Application::init(const std::string title, const int width, const int height, const int scale)
{
    m_width = width;
    m_height = height;
    m_scale = scale;

    setup_working_directory();

	CreateConsoleWindow();

    setup_logging();

	m_renderwindow.reset(new sf::RenderWindow(
        sf::VideoMode(m_width * m_scale, m_height * m_scale), title
	));
    if (!m_renderwindow)
    {
        SPDLOG_ERROR("Error creating window");
        return false;
    }

    m_rendertexture.create(m_width, m_height);

    m_rendersprite.setTexture(m_rendertexture);
    m_rendersprite.setScale(m_scale, m_scale);

    if(!load_font())
    {
        return false;
    }

	SPDLOG_INFO("PixelWolf initialized.");
    return true;
}

void Application::run()
{
    running = true;

    if (!OnUserCreate()) running = false;

    while (m_renderwindow.get()->isOpen())
    {
        event();

        update();

        OnUserUpdate(2.0f);

        render();
    }
    OnUserDestroy();
}

void Application::event()
{
    sf::Event event;
    while (m_renderwindow.get()->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_renderwindow.get()->close();
    }
}

void Application::update()
{
}

void Application::render()
{
    m_renderwindow.get()->clear(sf::Color::Black);

    OnUserRender();

    m_renderwindow.get()->draw(m_rendersprite);

    m_renderwindow.get()->display();
}
