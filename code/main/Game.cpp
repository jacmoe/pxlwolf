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
#include "Thor/Math.hpp"

Game::Game()
: m_animator(m_animation_map)
, m_raycaster()
{
    m_map = std::make_shared<utility::Map>();
    m_atlas = std::make_shared<utility::ImageAtlas>();
}

Game::~Game()
{
}

bool Game::OnUserCreate()
{
    //utility::ImageAtlas* atlas = m_atlas.get();
    Pixelator* pixelator = m_pixelator.get();

    m_map.get()->init("assets/levels/pxlwolf.ldtk");

    m_map.get()->load("Level4");

#if 0
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
#endif

    if(m_map->loaded())
    {
        init_player(m_map.get()->player_start().x, m_map.get()->player_start().y, m_map.get()->player_heading(), thor::Pi / 2, 36);

        m_raycaster.init(m_width, m_height, m_map, m_pixelator);

        m_raycaster.generateAngleValues(m_width, &m_player.camera);

        m_raycaster.resetDepthBuffer();

        m_raycaster.raycastRender(&m_player.camera, 0.01);

        m_raycaster.renderBuffer();

        pixelator->addBuffer("minimap");
        pixelator->setSize("minimap", sf::Vector2i(m_map.get()->width() * 2, m_map.get()->height() * 2));

        m_raycaster.drawMinimap("minimap", m_player.camera, 2);

        pixelator->copy("pixelBuffer", 0, 0, true);

        pixelator->copy("minimap", 200, 0, true);
    }
    // m_anim_sprite.setScale(sf::Vector2f(10,10));
    // m_animator.play() << "roll";

    m_action_map["forward"] = thor::Action(sf::Keyboard::W);
    m_action_map["backward"] = thor::Action(sf::Keyboard::S);
    m_action_map["l_strafe"] = thor::Action(sf::Keyboard::A);
    m_action_map["r_strafe"] = thor::Action(sf::Keyboard::D);
    m_action_map["jump"] = thor::Action(sf::Keyboard::Space);
    m_action_map["crouch"] = thor::Action(sf::Keyboard::LControl);
    m_action_map["sprint"] = thor::Action(sf::Keyboard::LShift);
    m_action_map["pause"] = thor::Action(sf::Keyboard::Pause);
    m_action_map["kill"] = thor::Action(sf::Keyboard::K);
    m_action_map["respawn"] = thor::Action(sf::Keyboard::R);
    m_action_map["map"] = thor::Action(sf::Keyboard::M);

    return true;
}

void Game::init_player(double x, double y, double angle, double fov, double viewDist)
{
    m_player.x = x;
    m_player.y = y;
    m_player.h = 0;
    m_player.angle = angle;
    m_player.health = 100; //PLACEHOLDER
    m_player.state = 1; //PLACEHOLDER
    m_player.spacePressed = 0;
    m_player.timer = 0;
    m_player.camera.x = m_player.x;
    m_player.camera.y = m_player.y;
    m_player.camera.angle = m_player.angle;
    m_player.camera.dist = viewDist;
    m_player.camera.fov = fov;
    m_player.camera.h = m_player.h;
}

void Game::update_player(sf::Time elapsedTime)
{
    int borderWidth = 2;

    // Jump test
    m_player.h += m_player.velH * elapsedTime.asSeconds();
    if (m_player.state && (m_action_map.isActive("jump")) && !m_player.h)
    {
        m_player.velH = 2.8;
    }
    if (m_player.h < m_player.groundH)
    {
        m_player.velH = 0;
        m_player.h = m_player.groundH;
    }
    else if (m_player.h > m_player.groundH)
    {
        m_player.velH -= 9.8 * elapsedTime.asSeconds();
    }

}

bool Game::OnUserUpdate(sf::Time elapsedTime)
{
    // m_animator.queue() << "roll";
    // m_animator.update(elapsedTime);
    // m_animator.animate(m_anim_sprite);

    update_player(elapsedTime);

    // const sf::RenderWindow* window = m_renderwindow.get();
    // sf::Mouse::setPosition(sf::Vector2i(m_width / 2, m_height / 2), *window);
    // sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    // m_camera.angle += 0.1 * elapsedTime.asMilliseconds() * mousePos.x;

    if(m_map->loaded())
    {
        m_pixelator.get()->clear();

        m_raycaster.resetDepthBuffer();

        m_raycaster.raycastRender(&m_player.camera, 0.01);

        m_raycaster.renderBuffer();

        m_raycaster.drawMinimap("minimap", m_player.camera, 2);

        m_pixelator.get()->copy("pixelBuffer", 0, 0, true);

        m_pixelator.get()->copy("minimap", 200, 0, true);
    }
    // sf::IntRect sprite_rect =  m_anim_sprite.getTextureRect();
    // SPDLOG_INFO("Sprite rect is : {},{},{},{}", sprite_rect.left, sprite_rect.top, sprite_rect.width, sprite_rect.height);    

    return true;
}

bool Game::OnUserRender()
{
    // m_pixelator.get()->copy("pixelBuffer", 0, 0, true);

    // m_renderwindow.get()->draw(m_anim_sprite);
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}
