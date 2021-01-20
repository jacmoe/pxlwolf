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
#include <SFML/Graphics.hpp>

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
};

class Game : public Application
{
public:
	Game();
	~Game();

	bool OnUserCreate() override;
	bool OnUserUpdate(sf::Time elapsedTime) override;
	bool OnUserRender() override;
	bool OnUserDestroy() override;
};
