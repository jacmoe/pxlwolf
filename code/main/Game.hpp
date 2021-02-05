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

struct _Camera
{
    double x;
    double y;
    double z;
    double h;
    double angle;
    double dirX;
    double dirY;
    double dirZ;
    double planeX;
    double planeY;
    double pitch;
};

struct _Player
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
    _Camera camera;
};

class Game : public Application
{
public:
    Game();
    ~Game();

protected:
    bool OnUserCreate() override;
    bool OnUserUpdate(double fDeltaTime) override;
    bool OnUserRender() override;
    bool OnUserDestroy() override;
};
