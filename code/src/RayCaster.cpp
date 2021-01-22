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
// #include <SFML/Graphics/Color.hpp>
// #include <SFML/Graphics/Rect.hpp>
// #include <SFML/System/Vector2.hpp>

RayCaster::RayCaster(Map& map, Pixelator& pixelator)
: m_map(map)
, m_pixelator(pixelator)
{
    //
}

void RayCaster::drawMinimap(const Camera& camera, int blockSize)
{
    int row, col;
    sf::IntRect mapRect;
    mapRect.width = m_map.width() * blockSize;
    mapRect.height = m_map.height() * blockSize;
    sf::IntRect blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;

    int p_x = m_map.player_position().x;
    int p_y = m_map.player_position().y;

    /* Draw map tiles */
    for(row = 0; row < m_map.height(); row++)
    {
        for(col = 0; col < m_map.width(); col++)
        {
            blockRect.left = mapRect.left + col * blockSize;
            blockRect.top = mapRect.top + row * blockSize;
            if(m_map.get_walls()[row * m_map.width() + col] > 0)
            {
                sf::Color blockcolor = m_map.get_wall_element(m_map.get_walls()[row * m_map.width() + col]).color;
                m_pixelator.drawFilledRect(blockRect, blockcolor);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                sf::Color sepiaPink = {221,153,153,255};
                m_pixelator.drawFilledRect(blockRect, sepiaPink);
            }
        }
    }
}
