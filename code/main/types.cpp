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
#include "main/types.hpp"

Vector2i::Vector2i(int x, int y)
    : x(x)
    , y(y)
{
}

Vector2f::Vector2f(float x, float y)
    : x(x)
    , y(y)
{
}

Vector2d::Vector2d(double x, double y)
    : x(x)
    , y(y)
{
}

IntRect::IntRect(int left, int top, int width, int height)
    : left(left)
    , top(top)
    , width(width)
    , height(height)
{
}
