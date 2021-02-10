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
#include "stb_image.h"

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
    initDepthBuffer();

    tex_tile_width = 128;
    tex_tile_height = 128;
    ceil_tile_width = 512;
    ceil_tile_height = 512;

    initRayTexture("assets/textures/wolfsheet.png", tex_tile_width, tex_tile_height, 35);
    initWallCeilTexture("assets/textures/ceilfloor.png", ceil_tile_width, ceil_tile_height, 2);
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

uint32_t toIntColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)a);
}

SDL_Color toSDLColor(uint32_t pixColor)
{
    uint8_t r = (uint8_t)(pixColor >> 3*8);
    uint8_t g = (uint8_t)((pixColor >> 2*8) & 0xFF);
    uint8_t b = (uint8_t)((pixColor >> 8) & 0xFF);
    uint8_t a = (uint8_t)(pixColor & 0xFF);
    SDL_Color newColor = {r, g, b, a};
    return newColor;
}

/** to8BitColor
 * @brief Paletizes 32bit color to 8bit color
 * 
 * @param colorDat Raw truecolor value to paletize
 * @return 8 bit color value
 */
uint32_t to8BitColor(uint32_t colorDat)
{
    int r = (int)(colorDat >> 3*8);
    int g = (int)((colorDat >> 2*8) & 0xFF);
    int b = (int)((colorDat >> 8) & 0xFF);
    int newR = (int)ceil(round((double)r / 255.0*15) * (255.0/15));
    int newG = (int)ceil(round((double)g / 255.0*15) * (255.0/15));
    int newB = (int)ceil(round((double)b / 255.0*15) * (255.0/15));
    return (uint32_t)(newR) << 3*8 | (uint32_t)(newG) << 2*8 | (uint32_t)newB << 8 | (uint32_t)0xFF;
}


void RayCaster::drawMinimap(const std::string& buffer_name, const Camera& camera, int blockSize)
{
    utility::Map* map = m_map.get();

    int row, col;
    SDL_Rect mapRect;
    mapRect.w = map->width() * blockSize;
    mapRect.h = map->height() * blockSize;
    mapRect.x = mapRect.y = 0;
    mapRect.x = 220;
    SDL_Rect blockRect;
    blockRect.w = blockSize;
    blockRect.h = blockSize;

    int p_x = static_cast<int>(camera.x);
    int p_y = static_cast<int>(camera.y);
    // correct camera position
    // p_x = std::abs(p_x - map->width());

    /* Draw map tiles */
    for(row = 0; row < map->height(); row++)
    {
        for(col = 0; col < map->width(); col++)
        {
            blockRect.x = mapRect.x + col * blockSize;
            blockRect.y = mapRect.y + row * blockSize;
            if(map->walls()[row * map->width() + col] > 0)
            {
                uint32_t blockcolor = map->wall_element(map->walls()[row * map->width() + col]).color;
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, blockcolor);
            }
            else
            {
                uint32_t background_color = m_pixelator.get()->toIntColor(112,112,112,255);
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, background_color);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                uint32_t sepiaPink = m_pixelator.get()->toIntColor(221,153,153,255);
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, sepiaPink);
            }
        }
    }
}

uint32_t RayCaster::pixelGradientShader(uint32_t pixel, double percent, SDL_Color target)
{
    int r = (int)(pixel >> 3*8);
    int g = (int)((pixel >> 2*8) & 0xFF);
    int b = (int)((pixel >> 8) & 0xFF);
    int a = (int)(pixel & 0xFF);
    int dr = target.r - r;
    int dg = target.g - g;
    int db = target.b - b;
    int da = target.a - a;
    r += (int)((double)dr * percent);
    g += (int)((double)dg * percent);
    b += (int)((double)db * percent);
    a += (int)((double)da * percent);
    return toIntColor(r,g,b,a);
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
                    SDL_Color alphaColor = toSDLColor(color);
                    alphaColor.a *= alphaNum;
                    m_pixelator.get()->setPixel("alphaBuffer", x, y, toIntColor(alphaColor.r, alphaColor.g, alphaColor.b, alphaColor.a));
                }
                else
                {
                    m_pixelator.get()->setPixel("alphaBuffer", x, y, color);
                }
            }
            else
            {
                m_pixelator.get()->setPixel("alphaBuffer", x, y, color);
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
                m_pixelator.get()->setPixel("pixelBuffer", x, y, color);
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
    uint32_t pixel;
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
                             SDL_Color targetColor)
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
        int magic_number = 
            tileNum * tex_tile_width * tex_tile_height
            + (uint32_t)floor(((double)(offY + i) /
            (double)offH) * (tex_tile_height))
             * tex_tile_width + column;

        uint32_t pix = m_pixels[magic_number];
        if (pix & 0xFF)
        {
            pix = pixelGradientShader(pix, fadePercent, targetColor);
            setPixelAlphaDepth(x, i+y, pix, alphaNum, depth);
        }
    }
}

//! RayBuffer dependent
void RayCaster::raycastRender(Camera* camera, double resolution)
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
                SDL_Color colorDat = {0,0,0,255};
                if (rayLen != 0)
                {
                    uint8_t side;
                    double newX;
                    double newY;
                    double rayLen = sqrt(getInterDist(rayStepX, rayStepY, camera->x + rayOffX, camera->y + rayOffY, (double)coordX, (double)coordY, &newX, &newY, &side))/scaleFactor;
                    uint32_t texCoord;
                    if (side > 1)
                    {
                        texCoord = (uint32_t)floor((newX - coordX) * tex_tile_width);
                    }
                    else
                    {
                        texCoord = (uint32_t)floor((newY - coordY) * tex_tile_height);
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
                    SDL_Color fadeColor = {77,150,154,255};
                    drawTextureColumn(
                        i, startY, deltaY, depth,
                        mapTile - 1, 1.0, 
                        texCoord, colorGrad, fadeColor
                    );
                    // Check for texture column transparency
                    bool hasAlpha = false;
                    int border = 2;
                    for (uint32_t p = 0; p < tex_tile_height; p++)
                    {
                        if (( m_pixels[(mapTile-1)*tex_tile_width*tex_tile_height+texCoord+(tex_tile_width*p)] & 0xFF) < 0xFF)
                        {
                            collisions++;
                            if (side == 0) // Hit from left
                            {
                                rayX += 1;
                                rayY += rayStepY * (1.0/rayStepX);
                            }
                            else if (side == 1) // Hit from right
                            {
                                rayX -= 1;
                                rayY -= rayStepY * (1.0/rayStepX);
                            }
                            else if (side == 2) // Hit from top
                            {
                                rayX += rayStepX * (1.0/rayStepY);
                                rayY += 1;
                            }
                            else // Hit from bottom
                            {
                                rayX -= rayStepX * (1.0/rayStepY);
                                rayY -= 1;
                            }
                            if (rayX+rayOffX < -border)
                            {
                                rayOffX += map->width() + border * 2;
                            }
                            else if (rayX+rayOffX >= map->width() + border)
                            {
                                rayOffX -= map->width() + border * 2;
                            }
                            if (rayY+rayOffY < -border)
                            {
                                rayOffY += map->height() + border*2;
                            }
                            else if (rayY+rayOffY >= map->height() + border)
                            {
                                rayOffY -= map->height() + border*2;
                            }
                            rayLen = sqrt((rayX-camera->x)*(rayX-camera->x) + (rayY-camera->y)*(rayY-camera->y));
                            rayStep++;
                            hasAlpha = 1;
                            break;
                        }
                    }
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

void RayCaster::initRayTexture(const std::string& path, int tile_width, int tile_height, int num_tiles)
{
    int32_t mPixWidth;
    int32_t mPixHeight;
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, NULL, 0);
    if (!rgbaData)
    {
        fprintf(stderr, "FATAL: Could not load textures. Exiting...\n");
        return;
    }

    uint32_t newPix = 0;
    for (uint32_t p = 0; p < tile_width * tile_height * num_tiles; p++)
    {
        // Get each component
        for (uint8_t comp = 0; comp < 4; comp++)
        {
            newPix |= ((uint32_t)(rgbaData[p*4+comp]) << (8 * (3-comp)));
        }
        m_pixels.push_back(newPix);
        newPix = 0;
    }
    stbi_image_free(rgbaData);
}

void RayCaster::initWallCeilTexture(const std::string& path, int tile_width, int tile_height, int num_tiles)
{
    int32_t mPixWidth;
    int32_t mPixHeight;
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, NULL, 0);
    if (!rgbaData)
    {
        fprintf(stderr, "FATAL: Could not load textures. Exiting...\n");
        return;
    }

    uint32_t newPix = 0;
    for (uint32_t p = 0; p < tile_width * tile_height * num_tiles; p++)
    {
        // Get each component
        for (uint8_t comp = 0; comp < 4; comp++)
        {
            newPix |= ((uint32_t)(rgbaData[p*4+comp]) << (8 * (3-comp)));
        }
        m_wall_ceil_pixels.push_back(newPix);
        newPix = 0;
    }
    stbi_image_free(rgbaData);
}

void RayCaster::renderFloor(Camera* camera, double resolution)
{
    int tileNum = 0;
    double scaleFactor = (double)m_width / (double)m_height * 2.4;

    // Get initial coordinate position at top-left of floor space
    uint32_t startX = 0;
    uint32_t startY = m_height / 2;

    double pixelX;
    double pixelY;
    double pixelDist;
    double pixelDepth;
    double fadePercent;

    uint32_t texX;
    uint32_t texY;

    double startAngle = camera->angle - camera->fov / 2.0;
    double rayAngle;
    double rayCos;

    SDL_Color fadeColor = {50,20,50,255};
    
    // iterate through *all* pixels...
    for (int x = startX; x < m_width; x++)
    {
        // Establish angle of column...
        rayAngle = startAngle + camera->angleValues[x];
        rayCos = cos(rayAngle - camera->angle);

        for (int y = startY + 1; y < m_height; y++)
        {
            // Compute the distance to the pixel...
            pixelDist = (double)m_height * (1 + 2 * camera->h) / (10.0 * (y-startY-1) * rayCos) * scaleFactor;
            double fogConstant = 4.0/5;
            pixelDepth = (pixelDist * rayCos);
            fadePercent = pixelDist / (camera->dist * fogConstant);
            pixelX = camera->x + pixelDist * cos(rayAngle);
            pixelY = camera->y + pixelDist * sin(rayAngle);
            // Wow, is that really it? The math says so...
            int r;
            int g;
            int b;
            // r = 74;
            // g = 82;
            // b = 99;
            if (pixelDist < camera->dist * fogConstant)
            {
                // Get associated coordinate pixel...
                // TODO: some grid code...
                texX = (uint32_t)floor((double)ceil_tile_width * (pixelX - floor(pixelX)));
                texY = (uint32_t)floor((double)ceil_tile_height * (pixelY - floor(pixelY)));
                uint32_t pixColor = m_wall_ceil_pixels[tileNum * ceil_tile_width * ceil_tile_height + texX + texY * ceil_tile_width];
                r = (int)(pixColor >> 3*8);
                g = (int)((pixColor >> 2*8) & 0xFF);
                b = (int)((pixColor >> 8) & 0xFF);
                int dr = fadeColor.r - r;
                int dg = fadeColor.g - g;
                int db = fadeColor.b - b;
                r += (int)((double)dr * fadePercent);
                g += (int)((double)dg * fadePercent);
                b += (int)((double)db * fadePercent);
            }
            else
            {
                r = fadeColor.r;
                g = fadeColor.g;
                b = fadeColor.b;
            }
            // buffer->pixels[x + y * buffer->width] = ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)0xFF);
            m_pixelator.get()->setPixel(x, y, toIntColor(r, g, b, 255));
            // buffer->pixels[x + y * buffer->width] = ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)0xFF);
        }
    }
}

void RayCaster::renderCeiling(Camera* camera, double resolution)
{
    int tileNum = 1;
    double scaleFactor = (double)m_width / (double)m_height * 2.4;

    // Get initial coordinate position at top-left of floor space
    uint32_t startX = 0;
    uint32_t startY = 0;

    double pixelX;
    double pixelY;
    double pixelDist;
    double pixelDepth;
    double fadePercent;

    uint32_t texX;
    uint32_t texY;

    double startAngle = camera->angle - camera->fov / 2.0;
    double rayAngle;
    double rayCos;

    SDL_Color fadeColor = {50,20,50,255};
    
    // iterate through *all* pixels...
    for (int x = startX; x < m_width; x++)
    {
        // Establish angle of column...
        rayAngle = startAngle + camera->angleValues[x];
        rayCos = cos(rayAngle - camera->angle);

        for (int y = startY; y < m_height / 2; y++)
        {
            // Compute the distance to the pixel...
            pixelDist = (double)m_height * (1 - 2 * camera->h) / (10.0 * (m_height / 2 - y) * rayCos) * scaleFactor;
            pixelDepth = (pixelDist * rayCos);
            double fogConstant = 4.0/5;
            fadePercent = pixelDist / (camera->dist * fogConstant);
            pixelX = camera->x + pixelDist * cos(rayAngle);
            pixelY = camera->y + pixelDist * sin(rayAngle);
            // Wow, is that really it? The math says so...
            int r;
            int g;
            int b;
            // r = 110;
            // g = 118;
            // b = 135;
            if (pixelDist < camera->dist * fogConstant)
            {
                // Get associated coordinate pixel...
                // TODO: some grid code...
                texX = (uint32_t)floor((double)ceil_tile_width * (pixelX - floor(pixelX)));
                texY = (uint32_t)floor((double)ceil_tile_height * (pixelY - floor(pixelY)));
                uint32_t pixColor = m_wall_ceil_pixels[tileNum * ceil_tile_width * ceil_tile_height + texX + texY * ceil_tile_width];
                r = (int)(pixColor >> 3*8);
                g = (int)((pixColor >> 2*8) & 0xFF);
                b = (int)((pixColor >> 8) & 0xFF);
                int dr = fadeColor.r - r;
                int dg = fadeColor.g - g;
                int db = fadeColor.b - b;
                r += (int)((double)dr * fadePercent);
                g += (int)((double)dg * fadePercent);
                b += (int)((double)db * fadePercent);
            }
            else
            {
                r = fadeColor.r;
                g = fadeColor.g;
                b = fadeColor.b;
            }
            // PixelRenderer::drawPix(buffer, x, y, PixelRenderer::toPixColor(r,g,b,0xff));
            m_pixelator.get()->setPixel(x, y, toIntColor(r, g, b, 255));
        }
    }
}
