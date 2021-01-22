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

RayCaster::RayCaster(std::shared_ptr<Map> map, std::shared_ptr<Pixelator> pixelator)
: m_map(map)
, m_pixelator(pixelator)
{
    //
}

void RayCaster::drawMinimap(const std::string owner, const std::string name, const Camera& camera, int blockSize)
{
    m_pixelator.get()->addBuffer(name);
    m_pixelator.get()->setActiveBuffer(name);
    m_pixelator.get()->setSize(sf::Vector2i(m_map.get()->width() * 2, m_map.get()->height() * 2));

    int row, col;
    sf::IntRect mapRect;
    mapRect.width = m_map.get()->width() * blockSize;
    mapRect.height = m_map.get()->height() * blockSize;
    sf::IntRect blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;

    int p_x = m_map.get()->player_position().x;
    int p_y = m_map.get()->player_position().y;

    /* Draw map tiles */
    for(row = 0; row < m_map.get()->height(); row++)
    {
        for(col = 0; col < m_map.get()->width(); col++)
        {
            blockRect.left = mapRect.left + col * blockSize;
            blockRect.top = mapRect.top + row * blockSize;
            if(m_map.get()->get_walls()[row * m_map.get()->width() + col] > 0)
            {
                sf::Color blockcolor = m_map.get()->get_wall_element(m_map.get()->get_walls()[row * m_map.get()->width() + col]).color;
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
