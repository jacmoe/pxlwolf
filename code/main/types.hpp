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

template <typename T>
struct Vector2
{
    T x;
    T y;

    Vector2() = default;
    Vector2(T x, T y)
        : x(x)
        , y(y)
    {
    }
};

template <typename T>
struct Rect
{
    T left;
    T top;
    T width;
    T height;

    Rect() = default;
    Rect(T left, T top, T width, T height)
        : left(left)
        , top(top)
        , width(width)
        , height(height)
    {
    }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Rect<int> IntRect;
typedef Rect<float> FloatRect;
typedef Rect<double> DoubleRect;
