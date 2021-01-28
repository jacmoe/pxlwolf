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
#include "lua_main.hpp"
#include "physfs.hpp"
#include "RayCaster.hpp"

Game::Game()
: m_animator(m_animation_map)
{
    m_map = std::make_shared<utility::Map>();
    m_atlas = std::make_shared<utility::ImageAtlas>();
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    utility::ImageAtlas* atlas = m_atlas.get();
    Pixelator* pixelator = m_pixelator.get();

    m_map.get()->init("assets/levels/pxlwolf.ldtk");

    m_map.get()->load("Level1");

    atlas->load("assets/textures/wall.png", sf::Vector2u(64, 64));

    m_sprite_loader.load("assets/sprites/orc.toml");

    m_animation_map.addAnimation("stand_back", 
        m_sprite_loader.getAnimations().at("stand_back").first ,
        m_sprite_loader.getAnimations().at("stand_back").second);

    m_animation_map.addAnimation("stand_front_left", 
        m_sprite_loader.getAnimations().at("stand_front_left").first ,
        m_sprite_loader.getAnimations().at("stand_front_left").second);

    m_animation_map.addAnimation("stand_front", 
        m_sprite_loader.getAnimations().at("stand_front").first ,
        m_sprite_loader.getAnimations().at("stand_front").second);

    m_animation_map.addAnimation("stand_front_right", 
        m_sprite_loader.getAnimations().at("stand_front_right").first ,
        m_sprite_loader.getAnimations().at("stand_front_right").second);

    m_animation_map.addAnimation("attack_back", 
        m_sprite_loader.getAnimations().at("attack_back").first ,
        m_sprite_loader.getAnimations().at("attack_back").second);

    m_animation_map.addAnimation("attack_front_left", 
        m_sprite_loader.getAnimations().at("attack_front_left").first ,
        m_sprite_loader.getAnimations().at("attack_front_left").second);

    m_animation_map.addAnimation("attack_front", 
        m_sprite_loader.getAnimations().at("attack_front").first ,
        m_sprite_loader.getAnimations().at("attack_front").second);

    m_animation_map.addAnimation("attack_front_right", 
        m_sprite_loader.getAnimations().at("attack_front_right").first ,
        m_sprite_loader.getAnimations().at("attack_front_right").second);

    m_animation_map.addAnimation("walk_back", 
        m_sprite_loader.getAnimations().at("walk_back").first ,
        m_sprite_loader.getAnimations().at("walk_back").second);

    m_animation_map.addAnimation("walk_front_left", 
        m_sprite_loader.getAnimations().at("walk_front_left").first ,
        m_sprite_loader.getAnimations().at("walk_front_left").second);

    m_animation_map.addAnimation("walk_front", 
        m_sprite_loader.getAnimations().at("walk_front").first ,
        m_sprite_loader.getAnimations().at("walk_front").second);

    m_animation_map.addAnimation("walk_front_right", 
        m_sprite_loader.getAnimations().at("walk_front_right").first ,
        m_sprite_loader.getAnimations().at("walk_front_right").second);

    m_animation_map.addAnimation("punch_back", 
        m_sprite_loader.getAnimations().at("punch_back").first ,
        m_sprite_loader.getAnimations().at("punch_back").second);

    m_animation_map.addAnimation("punch_front_left", 
        m_sprite_loader.getAnimations().at("punch_front_left").first ,
        m_sprite_loader.getAnimations().at("punch_front_left").second);

    m_animation_map.addAnimation("punch_front", 
        m_sprite_loader.getAnimations().at("punch_front").first ,
        m_sprite_loader.getAnimations().at("punch_front").second);

    m_animation_map.addAnimation("punch_front_right", 
        m_sprite_loader.getAnimations().at("punch_front_right").first ,
        m_sprite_loader.getAnimations().at("punch_front_right").second);

    m_animation_map.addAnimation("idle_back", 
        m_sprite_loader.getAnimations().at("idle_back").first ,
        m_sprite_loader.getAnimations().at("idle_back").second);

    m_animation_map.addAnimation("idle_front_left", 
        m_sprite_loader.getAnimations().at("idle_front_left").first ,
        m_sprite_loader.getAnimations().at("idle_front_left").second);

    m_animation_map.addAnimation("idle_front", 
        m_sprite_loader.getAnimations().at("idle_front").first ,
        m_sprite_loader.getAnimations().at("idle_front").second);

    m_animation_map.addAnimation("idle_front_right", 
        m_sprite_loader.getAnimations().at("idle_front_right").first ,
        m_sprite_loader.getAnimations().at("idle_front_right").second);

    m_animation_map.addAnimation("roll", 
        m_sprite_loader.getAnimations().at("roll").first ,
        m_sprite_loader.getAnimations().at("roll").second);

    m_sprite_texture.loadFromFile("assets/sprites/orc.png");
    m_anim_sprite.setTexture(m_sprite_texture);
    m_anim_sprite.setPosition(sf::Vector2f(19,19));

    pixelator->addBuffer("secondary");

    pixelator->setActiveBuffer("secondary");

    pixelator->copy(atlas->getPixels(0), atlas->getTileSize(), 0, 0, sf::IntRect(0, 0, atlas->getTileSize().x, atlas->getTileSize().y));

    pixelator->setActiveBuffer("primary");

    pixelator->drawColumn(100, 2, 50, sf::Color::Blue);

    pixelator->drawRow(10, 10, 200, sf::Color::Red);

    pixelator->drawFilledRect(sf::IntRect(10, 10, 10, 10), sf::Color::Cyan);

    pixelator->drawLine(sf::Vector2i(10,10), sf::Vector2i(200, 80), sf::Color::Magenta);

    pixelator->drawCircle(sf::Vector2i(100,100), 80, sf::Color::Blue);

    pixelator->drawRect(sf::IntRect(60, 60, 100, 100), sf::Color::White);

    pixelator->setActiveBuffer("secondary");

    pixelator->copy("primary", 0, 0, true);

    m_action_map["test"] = thor::Action(sf::Mouse::Left, thor::Action::Hold);

    RayCaster raycaster(m_map, m_pixelator);

    if(m_map->loaded())
    {
        Camera camera;
        camera.x = m_map.get()->player_start().x;
        camera.y = m_map.get()->player_start().y;
        raycaster.drawMinimap("secondary", "minimap", camera, 2);

        pixelator->copy("minimap", 200, 0, true);
    }
    m_anim_sprite.setScale(sf::Vector2f(10,10));
    m_animator.play() << "roll";

    return true;
}

bool Game::OnUserUpdate(sf::Time elapsedTime)
{
    m_animator.queue() << "roll";
    m_animator.update(elapsedTime);
    m_animator.animate(m_anim_sprite);

    write_text("Hello from PixelWolf! Time is : " + std::to_string(elapsedTime.asSeconds()) + " Frames per second : " + std::to_string(m_frames_per_second));

    if (m_action_map.isActive("test"))
        write_text("Left mouse button down!!");

    // sf::IntRect sprite_rect =  m_anim_sprite.getTextureRect();
    // SPDLOG_INFO("Sprite rect is : {},{},{},{}", sprite_rect.left, sprite_rect.top, sprite_rect.width, sprite_rect.height);    

    return true;
}

bool Game::OnUserRender()
{
    m_renderwindow.get()->draw(m_anim_sprite);
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
