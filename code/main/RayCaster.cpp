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

RayCaster::RayCaster(std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator)
: m_map(map)
, m_pixelator(pixelator)
{
    initDepthBuffer(m_map->width(), m_map->height());
}

/** RaycasterEngine::generateAngleValues
 * @brief Creates a pregenerated list of angle offsets for Camera
 * TODO: Consolidate w/ Camera struct and GameEngine code 
 * @param width Width in pixels to generate offsets for
 * @param camera Camera for offsets
 */
void RayCaster::generateAngleValues(uint32_t width, Camera* camera)
{
    double adjFactor = (double)width / (2 * tan(camera->fov / 2));
    camera->angleValues[0] = atan((width / 2) / adjFactor) - atan((width / 2 - 1) / adjFactor);
    for (uint32_t i = 1; i < width; i++)
    {
        if(i < width / 2)
        {
            camera->angleValues[i] = camera->angleValues[i-1] + atan((width / 2 - i) / adjFactor) - atan((width / 2 - i - 1) / adjFactor);
        }
        else
        {
            camera->angleValues[i] = camera->angleValues[i-1] + atan((i + 1 - width / 2) / adjFactor) - atan((i - width / 2) / adjFactor);
        }
    }
}

void RayCaster::drawMinimap(const std::string& owner, const std::string& name, const Camera& camera, int blockSize)
{
    utility::Map* map = m_map.get();
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
            if(map->walls()[row * map->width() + col] > 0)
            {
                sf::Color blockcolor = map->wall_element(map->walls()[row * map->width() + col]).color;
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

/** getInterDist
 * @brief Compute linear interpolation of ray intersect with wall
 * 
 * @param dx x displacement along ray step
 * @param dy y displacement along ray step
 * @param xi Initial x coordinate
 * @param yi Initial y coordinate
 * @param coordX Grid tile x coordinate
 * @param coordY Grid tile y coordinate
 * @param newX Interpolated x coordinate
 * @param newY Interpolated y coordinate
 * @param side Numerical representation of side of intersect
 * @return double Adjusted distance to intersect
 */
double RayCaster::getInterDist(double dx, double dy, double xi, double yi, double coordX, double coordY, double* newX, double* newY, uint8_t* side)
{
    // Check side intercepts first
    double slope = (dy/dx);
    double leftCoord = slope * (coordX - xi) + yi;
    double rightCoord = slope * (coordX + 1 - xi) + yi;
    slope = (dx/dy);
    double topCoord = slope * (coordY - yi) + xi;
    double bottomCoord = slope * (coordY + 1 - yi) + xi;
    double dist;
    double minDist = -1;
    
    if ((int)floor(leftCoord) == (int)coordY) // Left side
    {
        minDist = (xi - coordX)*(xi - coordX) + (yi - leftCoord)*(yi - leftCoord);
        *side = 0;
        *newX = coordX;
        *newY = leftCoord;
    }
    dist = (coordX + 1 - xi)*(coordX + 1 - xi) + (rightCoord - yi)*(rightCoord - yi);
    if ((int)floor(rightCoord) == (int)coordY && (dist < minDist || minDist == -1)) // Right side
    {
        minDist = dist;
        *side = 1;
        *newX = coordX + 1;
        *newY = rightCoord;
    }
    dist = (xi - topCoord)*(xi - topCoord) + (yi - coordY)*(yi - coordY);
    if ((int)floor(topCoord) == (int)coordX && (dist < minDist || minDist == -1)) // Top side
    {
        minDist = dist;
        *side = 2;
        *newX = topCoord;
        *newY = coordY;
    }
    dist = (xi - bottomCoord)*(xi - bottomCoord) + (yi - coordY - 1)*(yi - coordY - 1);
    if ((int)floor(bottomCoord) == (int)coordX && (dist < minDist || minDist == -1)) // Bottom side
    {
        minDist = dist;
        *side = 3;
        *newX = bottomCoord;
        *newY = coordY + 1;
    }
    return minDist;
}

void RayCaster::initDepthBuffer(uint32_t width, uint32_t height)
{
    Pixelator* pixelator = m_pixelator.get();
    pixelator->addBuffer("pixelBuffer");
    pixelator->setSize("pixelBuffer", sf::Vector2i(width, height));

    pixelator->addBuffer("alphaBuffer");
    pixelator->setSize("alphaBuffer", sf::Vector2i(width, height));

    m_pixel_depth.assign(width * height, INFINITY);
    m_alpha_depth.assign(width * height, INFINITY);

    m_width = width;
    m_height = height;
}

void RayCaster::resetDepthBuffer()
{
    Pixelator* pixelator = m_pixelator.get();
    pixelator->clear("pixelBuffer");
    pixelator->clear("alphaBuffer");
    m_pixel_depth.assign(m_width * m_height, INFINITY);
    m_alpha_depth.assign(m_width * m_height, INFINITY);
}

double RayCaster::getDepth(uint32_t x, uint32_t y, uint8_t layer)
{
    return layer ? m_alpha_depth[m_width * y + x] : m_pixel_depth[m_width * y + x];
}

void RayCaster::setDepth(uint32_t x, uint32_t y, uint8_t layer, double depth)
{
    if (layer)
    {
        m_alpha_depth[m_width * y + x] = depth;
    }
    else
    {
        m_pixel_depth[m_width * y + x] = depth;
    }
}
