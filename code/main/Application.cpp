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

#include "dbg_console.hpp"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

const sf::Time Application::m_time_per_frame = sf::seconds(1.f/60.f);

Application::Application()
    : m_fullscreen(false)
    , m_font_name("assets/fonts/MedievalSharp-Bold.ttf")
    , m_font_size(16)
    , m_font_color({255, 255, 255, 255})
    , m_font()
    , m_scale(0)
    , m_width(0)
    , m_height(0)
    , m_aspect_ratio(360/240)
    , m_text_old_position(0.0f,0.0f)
    , m_render_offset(0.0f)
    , m_frames_per_second(0)
    , m_renderwindow(nullptr)
    , m_pixelator()
    , m_action_map()
    , m_stats_update_time()
    , m_stats_num_frames(0)
    , m_running(false)
    , m_text()
{}

Application::~Application()
{
    SPDLOG_INFO("PixelWolf shutdown.");
    m_renderwindow.reset();
    utility::closeConsoleWindow();
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

void Application::setup_logging()
{
    std::string logfile_name = "log/pxllog.txt";
    
    // Remove old log file
    if(std::filesystem::exists(logfile_name))
    {
        std::remove(logfile_name.c_str());
    }

    // Create console sink and file sink
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logfile_name, true);
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
        m_text_old_position = sf::Vector2f(10.f, 10.f);
        m_text.setCharacterSize(m_font_size);
        m_text.setString(m_title);
    }
    else
    {
        return false;
    }
    return true;
}

bool Application::init(const std::string title, int width, int height, const float scale, const bool fullscreen)
{
    m_width = width;
    m_height = height;
    m_scale = scale;
    m_title = title;
    m_fullscreen = fullscreen;
    m_aspect_ratio = static_cast<float>(m_width) / static_cast<float>(m_height);

    utility::CreateConsoleWindow();

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

    m_renderwindow.get()->setKeyRepeatEnabled(false);
    m_renderwindow.get()->setVerticalSyncEnabled(false);

    m_rendertexture.create(m_width, m_height);

    m_rendersprite.setTexture(m_rendertexture);

    if(m_fullscreen)
    {
        m_render_offset = (m_renderwindow.get()->getView().getSize().x - (m_renderwindow.get()->getView().getSize().y * m_aspect_ratio)) / 2;
        m_rendersprite.setScale(
        ((m_renderwindow.get()->getView().getSize().y * m_aspect_ratio) / m_rendersprite.getLocalBounds().width), 
        (m_renderwindow.get()->getView().getSize().y / m_rendersprite.getLocalBounds().height));
        m_rendersprite.setPosition(sf::Vector2f(m_render_offset, 0));
    }
    else
    {
        m_rendersprite.setScale(m_scale, m_scale);
        m_rendersprite.setPosition(sf::Vector2f(0, 0));
    }

    if(!load_font())
    {
        SPDLOG_ERROR("Error loading font");
        return false;
    }

    if(m_fullscreen)
    {
        m_text.setPosition(sf::Vector2f(m_text_old_position.x + m_render_offset, m_text_old_position.y));
    }

    m_pixelator = std::make_shared<Pixelator>();

    m_pixelator.get()->setSize(sf::Vector2i(m_width, m_height));

    m_action_map["quit"] = thor::Action(sf::Keyboard::LControl) && thor::Action(sf::Keyboard::Q);
    m_action_map["toggle_fullscreen"] = thor::Action(sf::Keyboard::LAlt) && thor::Action(sf::Keyboard::Enter);

    SPDLOG_INFO("PixelWolf initialized.");
    SPDLOG_INFO("Aspect ratio : {}", m_aspect_ratio);
    return true;
}

void Application::run()
{
    m_running = true;

    if (!OnUserCreate()) m_running = false;

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while ((m_renderwindow.get()->isOpen()) && m_running)
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > m_time_per_frame)
        {
            timeSinceLastUpdate -= m_time_per_frame;

            event();

            update(m_time_per_frame);

            OnUserUpdate(m_time_per_frame);
        }

        render();
    }
    OnUserDestroy();
}

void Application::toggle_fullscreen()
{
    if(m_fullscreen)
    {
        m_renderwindow.get()->create(
            sf::VideoMode(m_width * static_cast<unsigned int>(m_scale), m_height * static_cast<unsigned int>(m_scale)), m_title
            , sf::Style::Fullscreen
        );
        m_render_offset = (m_renderwindow.get()->getView().getSize().x - (m_renderwindow.get()->getView().getSize().y * m_aspect_ratio)) / 2;
        m_rendersprite.setScale(
        ((m_renderwindow.get()->getView().getSize().y * m_aspect_ratio) / m_rendersprite.getLocalBounds().width), 
        (m_renderwindow.get()->getView().getSize().y / m_rendersprite.getLocalBounds().height));
        m_rendersprite.setPosition(sf::Vector2f(m_render_offset, 0));
        m_text.setPosition(sf::Vector2f(m_text_old_position.x + m_render_offset, m_text_old_position.y));
    }
    else
    {
        m_renderwindow.get()->create(
            sf::VideoMode(m_width * static_cast<unsigned int>(m_scale), m_height * static_cast<unsigned int>(m_scale)), m_title
            , sf::Style::Default
        );
        m_rendersprite.setScale(m_scale, m_scale);
        m_rendersprite.setPosition(sf::Vector2f(0, 0));
        m_text.setPosition(m_text_old_position);
    }
    m_fullscreen = !m_fullscreen;
}

void Application::event()
{
     sf::Event event;
    while (m_renderwindow.get()->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_renderwindow.get()->close();

        m_action_map.pushEvent(event);

        if (m_action_map.isActive("quit"))
            m_running = false;

        if (m_action_map.isActive("toggle_fullscreen"))
            toggle_fullscreen();

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

    m_rendertexture.update(m_pixelator.get()->getPixelsPtr());

    m_renderwindow.get()->draw(m_rendersprite);
    m_renderwindow.get()->draw(m_text);

    OnUserRender();

    m_renderwindow.get()->display();
}
