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

const sf::Time Application::m_time_per_frame = sf::seconds(1.f/60.f);

Application::Application()
    : running(false)
    , m_scale(0)
    , m_width(0)
    , m_height(0)
    , m_fullscreen(false)
    , m_renderwindow(nullptr)
    , m_font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , m_font_size(16)
    , m_font_color({255, 255, 255, 255})
    , m_font()
    , m_text()
    , m_stats_update_time()
    , m_stats_num_frames(0)
    , m_frames_per_second(0)
    , m_pixelator()
{}

Application::~Application()
{
	SPDLOG_INFO("PixelWolf shutdown.");
    m_renderwindow.reset();
	closeConsoleWindow();
}

bool Application::OnUserCreate()
{
    return true;
}

bool Application::OnUserUpdate(sf::Time elapsedTime)
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
    m_text.setString(text);
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
    strList.push_back("\\vsbuild\\code\\");
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
    if(std::filesystem::exists(m_font_name))
    {
        m_font.loadFromFile(m_font_name);
        m_text.setFont(m_font);
        m_text.setFillColor(m_font_color);
        m_text.setPosition(10.f, 10.f);
        m_text.setCharacterSize(m_font_size);
        m_text.setString(m_title);
    }
    else
    {
        return false;
    }
    return true;
}

bool Application::init(const std::string title, const int width, const int height, const float scale, const bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_scale = scale;
    m_title = title;
    m_fullscreen = fullscreen;

    setup_working_directory();

	CreateConsoleWindow();

    setup_logging();

    if(m_fullscreen)
    {
        m_renderwindow.reset(new sf::RenderWindow(
            sf::VideoMode(m_width * static_cast<unsigned int>(m_scale), m_height * static_cast<unsigned int>(m_scale)), m_title
            , sf::Style::Fullscreen
        ));
    }
    else
    {
        m_renderwindow.reset(new sf::RenderWindow(
            sf::VideoMode(m_width * static_cast<unsigned int>(m_scale), m_height * static_cast<unsigned int>(m_scale)), m_title
            , sf::Style::Default
        ));
    }
    if (!m_renderwindow)
    {
        SPDLOG_ERROR("Error creating window");
        return false;
    }

    m_rendertexture.create(m_width, m_height);

    m_rendersprite.setTexture(m_rendertexture);

    if(!m_fullscreen)
    {
        m_rendersprite.setScale(m_scale, m_scale);
    }

    if(!load_font())
    {
        return false;
    }

    m_pixelator.setSize(sf::Vector2f(static_cast<float>(m_width), static_cast<float>(m_height)));

	SPDLOG_INFO("PixelWolf initialized.");
    return true;
}

void Application::run()
{
    running = true;

    if (!OnUserCreate()) running = false;

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while ((m_renderwindow.get()->isOpen()) && running)
    {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > m_time_per_frame)
		{
			timeSinceLastUpdate -= m_time_per_frame;

            event();

            update(m_time_per_frame);

            OnUserUpdate(m_time_per_frame);
            
            running = handle_input();
		}

        render();
    }
    OnUserDestroy();
}

bool Application::handle_input()
{
    bool status = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        status = false;
    }
    return status;
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

void Application::update(sf::Time elapsedTime)
{
	m_stats_update_time += elapsedTime;
	m_stats_num_frames += 1;

	if (m_stats_update_time >= sf::seconds(1.0f))
	{
        m_frames_per_second = m_stats_num_frames;
		m_stats_update_time -= sf::seconds(1.0f);
		m_stats_num_frames = 0;
	}
}

void Application::render()
{
    m_renderwindow.get()->clear(sf::Color::Black);

    OnUserRender();

    m_rendertexture.update(m_pixelator.getPixelsPtr());

    m_renderwindow.get()->draw(m_rendersprite);
    m_renderwindow.get()->draw(m_text);

    m_renderwindow.get()->display();
}
