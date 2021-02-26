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

struct Vector2i
{
    int x;
    int y;

    Vector2i() = default;
    Vector2i(int x, int y);
};

struct Vector2f
{
    float x;
    float y;

    Vector2f() = default;
    Vector2f(float x, float y);
};

struct Vector2d
{
    float x;
    float y;

    Vector2d() = default;
    Vector2d(double x, double y);
};

struct IntRect
{
    int left;
    int top;
    int width;
    int height;

    IntRect() = default;
    IntRect(int left, int top, int width, int height);
};
