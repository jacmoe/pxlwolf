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
#include <cmath>
#include "RayCaster.hpp"
#include "Game.hpp"

RayCaster::RayCaster()
{
}

RayCaster::~RayCaster()
{
}

void RayCaster::init(uint32_t width, uint32_t height, std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator)
{
    m_map = map;
    m_pixelator = pixelator;
    m_width = width;
    m_height = height;
}

void RayCaster::drawMinimap(const std::string& buffer_name, const _Camera& camera, int blockSize)
{
    utility::Map* map = m_map.get();

    int row, col;
    Rectangle mapRect;
    mapRect.width = map->width() * blockSize;
    mapRect.height = map->height() * blockSize;
    mapRect.x = 0;
    mapRect.y = 0;
    Rectangle blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;
    blockRect.x = 0;
    blockRect.y = 0;

    int p_x = static_cast<int>(camera.x);
    int p_y = static_cast<int>(camera.y);

    /* Draw map tiles */
    for(row = 0; row < map->width(); row++)
    {
        for(col = 0; col < map->height(); col++)
        {
            blockRect.x = mapRect.x + col * blockSize;
            blockRect.y = ((mapRect.height - blockSize) - row * blockSize);// + map->width()();
            if(map->walls()[row * map->width() + col] > 0)
            {
                Color blockcolor = map->wall_element(map->walls()[row * map->width() + col]).color;
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, blockcolor);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                Color sepiaPink = {221,153,153,255};
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, sepiaPink);
            }
        }
    }
}

uint32_t RayCaster::pixelGradientShader(uint32_t pixel, double percent, Color target)
{
    Color source = GetColor(pixel);
    int dr = target.r - source.r;
    int dg = target.g - source.g;
    int db = target.b - source.b;
    int da = target.a - source.a;
    source.r += (int)((double)dr * percent);
    source.g += (int)((double)dg * percent);
    source.b += (int)((double)db * percent);
    source.a += (int)((double)da * percent);
    return ColorToInt(source);
}
