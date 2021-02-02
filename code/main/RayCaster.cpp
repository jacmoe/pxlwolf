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

void RayCaster::init(uint32_t width, uint32_t height, std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator)
{
    m_map = map;
    m_pixelator = pixelator;
    m_width = width;
    m_height = height;
    initDepthBuffer();
}

/** RaycasterEngine::generateAngleValues
 * @brief Creates a pregenerated list of angle offsets for Camera
 * TODO: Consolidate w/ Camera struct and GameEngine code 
 * @param width Width in pixels to generate offsets for
 * @param camera Camera for offsets
 */
void RayCaster::generateAngleValues(uint32_t width, _Camera* camera)
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
            blockRect.y = ((mapRect.height - blockSize) - row * blockSize);// + map->width();
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

/**
 * @brief 
 * 
 * @param buffer 
 * @param x 
 * @param y 
 * @param color 
 * @param alphaNum 
 * @param depth 
 */
void RayCaster::setPixelAlphaDepth(uint32_t x, uint32_t y, uint32_t color, double alphaNum, double depth)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        // Keep pixel in alpha layer
        if (alphaNum < 1 || (color & 0xff) < 255)
        {
            // If new alpha in front
            double pixDepth = getDepth(x, y, BL_ALPHA);
            if (pixDepth > depth)
            {
                setDepth(x, y, BL_ALPHA, depth);
                if (pixDepth == INFINITY)
                {
                    Color alphaColor = GetColor(color);
                    alphaColor.a *= alphaNum;
                    m_pixelator.get()->setPixel("alphaBuffer", x, y, alphaColor);
                }
                else
                {
                    m_pixelator.get()->setPixel("alphaBuffer", x, y, GetColor(color));
                }
            }
            else
            {
                m_pixelator.get()->setPixel("alphaBuffer", x, y, GetColor(color));
                //PixelRenderer::drawPix(buffer->alphaBuffer, x, y, PixelRenderer::blendAlpha(color, PixelRenderer::getPix(buffer->alphaBuffer, x, y), 1));
            }
        }
        // For opaque layer
        else
        {
            // If new pixel in front
            if (getDepth(x, y, BL_BASE) > depth)
            {
                setDepth(x, y, BL_BASE, depth);
                m_pixelator.get()->setPixel("pixelBuffer", x, y, GetColor(color));
                //PixelRenderer::drawPix(buffer->pixelBuffer, x, y, color);
            }
        }
    }
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

void RayCaster::initDepthBuffer()
{
    Pixelator* pixelator = m_pixelator.get();
    pixelator->addBuffer("pixelBuffer", m_width, m_height);

    pixelator->addBuffer("alphaBuffer", m_width, m_height);

    m_pixel_depth.assign(m_width * m_height, INFINITY);
    m_alpha_depth.assign(m_width * m_height, INFINITY);

    TraceLog(LOG_INFO,"Depthbuffer initialized.");
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

/** RaycasterEngine::renderBuffer
 * @brief Merges opaque and alpha layers of buffer for rendering
 * 
 * @param buffer Buffer to render
 */
void RayCaster::renderBuffer()
{
    Color pixel = BLANK;
    for (uint32_t i = 0; i < m_width; i++)
    {
        for (uint32_t j = 0; j < m_height; j++)
        {
            if (getDepth(i, j, BL_BASE) > getDepth(i, j, BL_ALPHA))
            {
                pixel = m_pixelator.get()->getPixel("alphaBuffer", i, j);
                m_pixelator.get()->setPixel("pixelBuffer", i, j, pixel);
            }
        }
    }
}

void RayCaster::drawTextureColumn(uint32_t x, int32_t y,
                              int32_t h, double depth,
                             uint8_t tileNum, double alphaNum, 
                             uint32_t column, double fadePercent, 
                             Color targetColor)
{
    if (y + h < 0 || fadePercent > 1.0)
    {
        return;  // Sorry, messy fix but it works
    }
    int32_t offH = h;
    int32_t offY = 0;
    if (y < 0)
    {
        offY = -y;
        h = h + y;
        y = 0;
    }
    if (y + h > m_height)
    {
        h = m_height - y;
    }

    for (int32_t i = 0; i < h; i++)
    {
        // Calculate pixel to draw from texture
        // uint32_t pix = texture->pixData[\
        //     tileNum*texture->tileWidth*texture->tileHeight + \
        //     (uint32_t)floor(((double)(offY + i)/(double)offH) * \
        //     (texture->tileHeight)) * texture->tileWidth + column];
        uint32_t pix = ColorToInt(m_map.get()->wall_element(tileNum + 1).color);
        // uint32_t pix = ColorToInt(RAYWHITE);//toIntColor(RAYWHITE.r, RAYWHITE.g, RAYWHITE.b, RAYWHITE.a);
        if (pix & 0xFF)
        {
            pix = pixelGradientShader(pix, fadePercent, targetColor);
            setPixelAlphaDepth(x, i+y, pix, alphaNum, depth);
        }
    }
}

//! RayBuffer dependent
void RayCaster::raycastRender(_Camera* camera, double resolution)
{
    // Establish starting angle and sweep per column
    double startAngle = camera->angle - camera->fov / 2.0;
    double adjFactor = m_width / (2 * tan(camera->fov / 2));
    double scaleFactor = (double)m_width / (double)m_height * 2.4;
    double rayAngle = startAngle;
    
    utility::Map* map = m_map.get();

    // Sweeeeep for each column
    #pragma omp parallel for schedule(dynamic,1) private(rayAngle)
    for (uint32_t i = 0; i < m_width; i++)
    {
        rayAngle = startAngle + camera->angleValues[i];
        double rayX = camera->x;
        double rayY = camera->y;
        double rayStepX = (resolution) * cos(rayAngle);
        double rayStepY = (resolution) * sin(rayAngle);
        double stepLen = (resolution) / scaleFactor;
        double rayLen = 0;
        int rayStep = 0;
        int rayOffX = 0;
        int rayOffY = 0;
        int collisions = 0;
        while (rayLen < camera->dist && collisions < 3)
        {
            int coordX = (int)floor(rayX+rayOffX);
            int coordY = (int)floor(rayY+rayOffY);
            if ((coordX >= 0.0 && coordY >= 0.0) && (coordX < map->width() && coordY < map->height()) && (map->walls()[coordY * map->width() + coordX] != 0))
            {
                uint8_t mapTile = map->walls()[coordY * map->width() + coordX];
                Color colorDat = {0,0,0,255};
                if (rayLen != 0)
                {
                    uint8_t side;
                    double newX;
                    double newY;
                    double rayLen = sqrt(getInterDist(rayStepX, rayStepY, camera->x + rayOffX, camera->y + rayOffY, (double)coordX, (double)coordY, &newX, &newY, &side))/scaleFactor;
                    uint32_t texCoord;
                    if (side > 1)
                    {
                        texCoord = (uint32_t)floor((newX - coordX) * 64);// * texData->tileWidth);
                    }
                    else
                    {
                        texCoord = (uint32_t)floor((newY - coordY) * 64);// * texData->tileWidth);
                    }
                    double depth = (double)(rayLen * cos(rayAngle - camera->angle));
                    //double colorGrad = (depth) / camera->dist;
                    //* Note: This is an awful mess but it is a temporary fix to get around rounding issues
                    int32_t drawHeight = (int32_t)ceil((double)m_height / (depth * 5));
                    int32_t wallHeight = (int32_t)round(-camera->h * drawHeight);
                    int32_t startY = m_height / 2 - drawHeight / 2 - wallHeight;
                    int32_t offsetStartY = m_height / 2 - drawHeight / 2;
                    int32_t deltaY = m_height - offsetStartY * 2;
                    //SDL_Color fadeColor = {77,150,154,255};
                    double colorGrad;
                    double fogConstant = 1.5/5;
                    if (rayLen < (camera->dist*fogConstant))
                    {
                        colorGrad = (rayLen) / (camera->dist*fogConstant);
                    }
                    else
                    {
                        colorGrad = 1.0;
                    }
                    Color FOG_COLOR = {50,20,50,255};
                    drawTextureColumn(
                        i, startY, deltaY, depth,
                        mapTile - 1, 1.0, 
                        texCoord, colorGrad, FOG_COLOR
                    );
                    // Check for texture column transparency
                    bool hasAlpha = false;
                    // for (uint32_t p = 0; p < texData->tileHeight; p++)
                    // {
                    //     if ((texData->pixData[(mapTile-1)*texData->tileWidth*texData->tileHeight+texCoord+(texData->tileWidth*p)] & 0xFF) < 0xFF)
                    //     {
                    //         collisions++;
                    //         if (side == 0) // Hit from left
                    //         {
                    //             rayX += 1;
                    //             rayY += rayStepY * (1.0/rayStepX);
                    //         }
                    //         else if (side == 1) // Hit from right
                    //         {
                    //             rayX -= 1;
                    //             rayY -= rayStepY * (1.0/rayStepX);
                    //         }
                    //         else if (side == 2) // Hit from top
                    //         {
                    //             rayX += rayStepX * (1.0/rayStepY);
                    //             rayY += 1;
                    //         }
                    //         else // Hit from bottom
                    //         {
                    //             rayX -= rayStepX * (1.0/rayStepY);
                    //             rayY -= 1;
                    //         }
                    //         if (rayX+rayOffX < -map->border)
                    //         {
                    //             rayOffX += map->width + map->border * 2;
                    //         }
                    //         else if (rayX+rayOffX >= map->width + map->border)
                    //         {
                    //             rayOffX -= map->width + map->border * 2;
                    //         }
                    //         if (rayY+rayOffY < -map->border)
                    //         {
                    //             rayOffY += map->height + map->border*2;
                    //         }
                    //         else if (rayY+rayOffY >= map->height + map->border)
                    //         {
                    //             rayOffY -= map->height + map->border*2;
                    //         }
                    //         rayLen = sqrt((rayX-camera->x)*(rayX-camera->x) + (rayY-camera->y)*(rayY-camera->y));
                    //         rayStep++;
                    //         hasAlpha = 1;
                    //         break;
                    //     }
                    // }
                    if (hasAlpha)
                    {
                        continue;
                    }
                }
                break;
            }
            rayX += rayStepX;
            rayY += rayStepY;
            int map_border = 2;
            if (rayX+rayOffX < - map_border)
            {
                rayOffX += map->width() + map_border * 2;
            }
            else if (rayX+rayOffX >= map->width() + map_border)
            {
                rayOffX -= map->width() + map_border * 2;
            }
            if (rayY+rayOffY < -map_border)
            {
                rayOffY += map->height() + map_border*2;
            }
            else if (rayY+rayOffY >= map->height() + map_border)
            {
                rayOffY -= map->height() + map_border * 2;
            }
            rayStep++;
            rayLen += stepLen;
        }
    }
}
