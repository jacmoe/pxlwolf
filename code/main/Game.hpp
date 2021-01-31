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
#pragma once

#include "Application.hpp"
#include "Pixelator.hpp"
#include "ImageAtlas.hpp"
#include "Map.hpp"
#include "RayCaster.hpp"

#include <SFML/Graphics.hpp>
#include "SpriteSheetLoader.hpp"
#include "Thor/Animations/Animator.hpp"

struct Camera
{
	double x;
	double y;
	double h;
	double angle;
	double dist;
	double fov;
	double angleValues[1024];
};

struct Player
{
	double x;
	double y;
    double h;
    double groundH;
    double velX;
    double velY;
    double velH;
	double angle;
    uint8_t usingMouse;
    uint8_t health;
    uint8_t state;
    double timer;
    uint8_t spacePressed;
	Camera camera;
};

class Game : public Application
{
public:
	Game();
	~Game();

protected:
	bool OnUserCreate() override;
	bool OnUserUpdate(sf::Time elapsedTime) override;
	bool OnUserRender() override;
	bool OnUserDestroy() override;

private:
    std::shared_ptr<utility::Map> m_map;
    std::shared_ptr<utility::ImageAtlas> m_atlas;

    utility::SpriteSheetLoader m_sprite_loader;
	thor::AnimationMap<sf::Sprite, std::string> m_animation_map;
    thor::Animator<sf::Sprite, std::string> m_animator;
	sf::Sprite m_anim_sprite;
    sf::Texture m_sprite_texture;
	Player m_player;
	RayCaster m_raycaster;

	void init_player(double x, double y, double angle, double fov, double viewDist);
	void update_player(sf::Time elapsedTime);
};
