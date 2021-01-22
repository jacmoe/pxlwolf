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
#include "RayCaster.hpp"
#include "Game.hpp"

RayCaster::RayCaster(std::shared_ptr<Map> map, std::shared_ptr<Pixelator> pixelator)
: m_map(map)
, m_pixelator(pixelator)
{}

void RayCaster::drawMinimap(const std::string& owner, const std::string& name, const Camera& camera, int blockSize)
{
    Map* map = m_map.get();
    m_pixelator.get()->addBuffer(name);
    m_pixelator.get()->setActiveBuffer(name);
    m_pixelator.get()->setSize(sf::Vector2i(map->width() * 2, map->height() * 2));

    int row, col;
    sf::IntRect mapRect;
    mapRect.width = map->width() * blockSize;
    mapRect.height = map->height() * blockSize;
    sf::IntRect blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;

    int p_x = static_cast<int>(camera.x);
    int p_y = static_cast<int>(camera.y);

    /* Draw map tiles */
    for(row = 0; row < map->height(); row++)
    {
        for(col = 0; col < map->width(); col++)
        {
            blockRect.left = mapRect.left + col * blockSize;
            blockRect.top = mapRect.top + row * blockSize;
            if(map->get_walls()[row * map->width() + col] > 0)
            {
                sf::Color blockcolor = map->get_wall_element(map->get_walls()[row * map->width() + col]).color;
                m_pixelator.get()->drawFilledRect(blockRect, blockcolor);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                sf::Color sepiaPink = {221,153,153,255};
                m_pixelator.get()->drawFilledRect(blockRect, sepiaPink);
            }
        }
    }
    m_pixelator.get()->setActiveBuffer(owner);
}
