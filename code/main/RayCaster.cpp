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
#   (c) 2020 - 2025 Jacob Moena
#
#   MIT License
#*/
#include <cmath>
#include "stb_image.h"
#include "spdlog/spdlog.h"
#include "main/RayCaster.hpp"
#include "main/types.hpp"
#include "utility/packer.hpp"
//#include "utility/unpacker.hpp"

RayCaster::RayCaster(): m_width(0), m_height(0), tex_tile_width(0), tex_tile_height(0), ceil_tile_width(0),
                        ceil_tile_height(0)
{
}

RayCaster::~RayCaster()
{
}
void RayCaster::init(const uint32_t width, const uint32_t height, const std::shared_ptr<utility::Map>& map, const std::shared_ptr<Pixelator>& pixelator)
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
void RayCaster::generateAngleValues(const uint32_t width, Camera* camera)
{
    const double adjFactor = static_cast<double>(width) / (2 * tan(camera->fov / 2));
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

void RayCaster::drawMinimap(const std::string& buffer_name, const Camera& camera, const int blockSize) const
{
    utility::Map* map = m_map.get();

    IntRect mapRect;
    mapRect.width = map->width() * blockSize;
    mapRect.height = map->height() * blockSize;
    mapRect.top = mapRect.top = 0;
    mapRect.left = 0;
    IntRect blockRect;
    blockRect.width = blockSize;
    blockRect.height = blockSize;

    const int p_x = static_cast<int>(camera.x);
    const int p_y = static_cast<int>(camera.y);

    /* Draw map tiles */
    for(int row = 0; row < map->height(); row++)
    {
        for(int col = 0; col < map->width(); col++)
        {
            //static_cast<uint8_t>(row);
            //const uint8_t p_col = static_cast<uint8_t>(col);
            //uint32_t p_tile = utility::pack(p_col,p_row,1,1);

            blockRect.left = mapRect.left + col * blockSize;
            blockRect.top = mapRect.top + row * blockSize;
            if( (map->walls()[row * map->width() + col] > 0) /*&& ( m_global_visited.find(p_tile) != m_global_visited.end() )*/)
            {
                ALLEGRO_COLOR blockcolor = map->wall_element(map->walls()[row * map->width() + col]).color;
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, blockcolor);
            }
            else
            {
                ALLEGRO_COLOR background_color = al_map_rgba(112,112,112,255);
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, background_color);
            }
            if(p_y == row && p_x == col)
            {
                /* Draw the player */
                ALLEGRO_COLOR sepiaPink = al_map_rgba(221,153,153,255);
                m_pixelator.get()->drawFilledRect(buffer_name, blockRect, sepiaPink);
            }
        }
    }
}

ALLEGRO_COLOR RayCaster::pixelGradientShader(const ALLEGRO_COLOR pixel, const double percent, const ALLEGRO_COLOR target)
{
    uint8_t r, g, b, a;
    al_unmap_rgba(pixel, &r, &g, &b, &a);
    uint8_t tr, tg, tb, ta;
    al_unmap_rgba(target, &tr, &tg, &tb, &ta);
    const uint8_t dr = tr - r;
    const uint8_t dg = tg - g;
    const uint8_t db = tb - b;
    const uint8_t da = ta - a;
    r += static_cast<uint8_t>(static_cast<double>(dr) * percent);
    g += static_cast<uint8_t>(static_cast<double>(dg) * percent);
    b += static_cast<uint8_t>(static_cast<double>(db) * percent);
    a += static_cast<uint8_t>(static_cast<double>(da) * percent);
    return al_map_rgba(r,g,b,a);
}

void RayCaster::setPixelAlphaDepth(const uint32_t x, const uint32_t y, const ALLEGRO_COLOR color, const double alphaNum, const double depth)
{
    uint8_t color_r, color_g, color_b, color_a;
    al_unmap_rgba(color, &color_r, &color_g, &color_b, &color_a);

    if (x < m_width && y < m_height)
    {
        // Keep pixel in alpha layer
        if (alphaNum < 1 || color_a < 255)
        {
            // If new alpha in front
            if (const double pixDepth = getDepth(x, y, BL_ALPHA); pixDepth > depth)
            {
                setDepth(x, y, BL_ALPHA, depth);
                if (pixDepth == INFINITY)
                {
                    color_a *= alphaNum;
                    m_pixelator.get()->setPixel("alphaBuffer", x, y, al_map_rgba(color_r, color_g, color_b, color_a));
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
double RayCaster::getInterDist(const double dx, const double dy, const double xi, const double yi, const double coordX, const double coordY, double* newX, double* newY, uint8_t* side)
{
    // Check side intercepts first
    double slope = (dy/dx);
    const double leftCoord = slope * (coordX - xi) + yi;
    const double rightCoord = slope * (coordX + 1 - xi) + yi;
    slope = (dx/dy);
    const double topCoord = slope * (coordY - yi) + xi;
    const double bottomCoord = slope * (coordY + 1 - yi) + xi;
    double minDist = -1;
    
    if (static_cast<int>(floor(leftCoord)) == static_cast<int>(coordY)) // Left side
    {
        minDist = (xi - coordX)*(xi - coordX) + (yi - leftCoord)*(yi - leftCoord);
        *side = 0;
        *newX = coordX;
        *newY = leftCoord;
    }
    double dist = (coordX + 1 - xi) * (coordX + 1 - xi) + (rightCoord - yi) * (rightCoord - yi);
    if (static_cast<int>(floor(rightCoord)) == static_cast<int>(coordY) && (dist < minDist || minDist == -1)) // Right side
    {
        minDist = dist;
        *side = 1;
        *newX = coordX + 1;
        *newY = rightCoord;
    }
    dist = (xi - topCoord)*(xi - topCoord) + (yi - coordY)*(yi - coordY);
    if (static_cast<int>(floor(topCoord)) == static_cast<int>(coordX) && (dist < minDist || minDist == -1)) // Top side
    {
        minDist = dist;
        *side = 2;
        *newX = topCoord;
        *newY = coordY;
    }
    dist = (xi - bottomCoord)*(xi - bottomCoord) + (yi - coordY - 1)*(yi - coordY - 1);
    if (static_cast<int>(floor(bottomCoord)) == static_cast<int>(coordX) && (dist < minDist || minDist == -1)) // Bottom side
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

double RayCaster::getDepth(const uint32_t x, const uint32_t y, const uint8_t layer) const
{
    return layer ? m_alpha_depth[m_width * y + x] : m_pixel_depth[m_width * y + x];
}

void RayCaster::setDepth(const uint32_t x, const uint32_t y, const uint8_t layer, const double depth)
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
 */
void RayCaster::renderBuffer() const
{
    for (uint32_t i = 0; i < m_width; i++)
    {
        for (uint32_t j = 0; j < m_height; j++)
        {
            if (getDepth(i, j, BL_BASE) > getDepth(i, j, BL_ALPHA))
            {
                ALLEGRO_COLOR pixel = m_pixelator.get()->getPixel("alphaBuffer", i, j);
                m_pixelator.get()->setPixel("pixelBuffer", i, j, pixel);
            }
        }
    }
}

void RayCaster::drawTextureColumn(const uint32_t x, int32_t y,
                              int32_t h, const double depth,
                             const uint8_t tileNum, const double alphaNum,
                             const uint32_t column, const double fadePercent,
                             const ALLEGRO_COLOR targetColor)
{
    if (y + h < 0 || fadePercent > 1.0)
    {
        return;  // Sorry, messy fix but it works
    }
    const int32_t offH = h;
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
        const int magic_number =
            tileNum * tex_tile_width * tex_tile_height
            + static_cast<uint32_t>(floor((static_cast<double>(offY + i) /
                static_cast<double>(offH)) * (tex_tile_height)))
             * tex_tile_width + column;

        const uint8_t* pixel = &m_pixels[magic_number * 4];
        ALLEGRO_COLOR pix = al_map_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
        //if (pix & 0xFF)
        //{
            pix = pixelGradientShader(pix, fadePercent, targetColor);
            setPixelAlphaDepth(x, i+y, pix, alphaNum, depth);
        //}
    }
}

void RayCaster::drawTextureColumnEx(const uint32_t x, int32_t y,
                              int32_t h, const double depth,
                              const Texture& texture,
                             const uint8_t tileNum, const double alphaNum,
                             const uint32_t column, const double fadePercent,
                             const ALLEGRO_COLOR targetColor)
{
    if (y + h < 0 || fadePercent > 1.0)
    {
        return;  // Sorry, messy fix but it works
    }
    const int32_t offH = h;
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
        const int magic_number =
            tileNum * texture.tile_width * texture.tile_height
            + static_cast<uint32_t>(floor((static_cast<double>(offY + i) /
                static_cast<double>(offH)) * (texture.tile_height)))
             * texture.tile_width + column;

        const uint8_t* pixel = &m_pixels[magic_number * 4];
        ALLEGRO_COLOR pix = al_map_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
        //if (pix & 0xFF)
        //{
            pix = pixelGradientShader(pix, fadePercent, targetColor);
            setPixelAlphaDepth(x, i+y, pix, alphaNum, depth);
        //}
    }
}

//! RayBuffer dependent
void RayCaster::raycastRender(const Camera* camera, const double resolution)
{
    // Establish starting angle and sweep per column
    const double startAngle = camera->angle - camera->fov / 2.0;
    //double adjFactor = m_width / (2 * tan(camera->fov / 2));
    const double scaleFactor = static_cast<double>(m_width) / static_cast<double>(m_height) * 2.4;
    double rayAngle = startAngle;
    
    utility::Map* map = m_map.get();

    // clear the set of visited tiles
    m_visited.clear();

    // Sweeeeep for each column
    #pragma omp parallel for schedule(dynamic,1) private(rayAngle)
    for (uint32_t i = 0; i < m_width; i++)
    {
        rayAngle = startAngle + camera->angleValues[i];
        double rayX = camera->x;
        double rayY = camera->y;
        const double rayStepX = (resolution) * cos(rayAngle);
        const double rayStepY = (resolution) * sin(rayAngle);
        const double stepLen = (resolution) / scaleFactor;
        double rayLen = 0;
        int rayStep = 0;
        int rayOffX = 0;
        int rayOffY = 0;
        int collisions = 0;
        while (rayLen < camera->dist && collisions < 3)
        {
            const int coordX = static_cast<int>(floor(rayX + rayOffX));
            const int coordY = static_cast<int>(floor(rayY + rayOffY));

            // pack visited tile into a 32 bit int
            const uint8_t theX = static_cast<uint8_t>(floor(rayX + rayOffX));
            const uint8_t theY = static_cast<uint8_t>(floor(rayY + rayOffY));
            uint32_t tilenum = utility::pack(theX,theY,1,1);
            // store it in the visited set
            m_visited.insert(tilenum);
            // store it in the global visited set
            m_global_visited.insert(tilenum);

            if ((coordX >= 0.0 && coordY >= 0.0) && (coordX < map->width() && coordY < map->height()) && (map->walls()[coordY * map->width() + coordX] != 0))
            {
                const uint8_t mapTile = map->walls()[coordY * map->width() + coordX];
                //ALLEGRO_COLOR colorDat = {0,0,0,255};
                if (rayLen != 0)
                {
                    uint8_t side;
                    double newX;
                    double newY;
                    rayLen = sqrt(getInterDist(rayStepX, rayStepY, camera->x + rayOffX, camera->y + rayOffY, coordX, coordY, &newX, &newY, &side))/scaleFactor;
                    uint32_t texCoord;
                    if (side > 1)
                    {
                        texCoord = static_cast<uint32_t>(floor((newX - coordX) * tex_tile_width));
                    }
                    else
                    {
                        texCoord = static_cast<uint32_t>(floor((newY - coordY) * tex_tile_height));
                    }
                    const double depth = rayLen * cos(rayAngle - camera->angle);
                    //double colorGrad = (depth) / camera->dist;
                    //* Note: This is an awful mess but it is a temporary fix to get around rounding issues
                    const int32_t drawHeight = static_cast<int32_t>(ceil(static_cast<double>(m_height) / (depth * 5)));
                    const int32_t wallHeight = static_cast<int32_t>(round(-camera->h * drawHeight));
                    const int32_t startY = m_height / 2 - drawHeight / 2 - wallHeight;
                    const int32_t offsetStartY = m_height / 2 - drawHeight / 2;
                    const int32_t deltaY = m_height - offsetStartY * 2;
                    //ALLEGRO_COLOR fadeColor = {77,150,154,255};
                    double colorGrad;
                    if (constexpr double fogConstant = 1.5/5; rayLen < (camera->dist*fogConstant))
                    {
                        colorGrad = (rayLen) / (camera->dist*fogConstant);
                    }
                    else
                    {
                        colorGrad = 1.0;
                    }
                    constexpr ALLEGRO_COLOR fadeColor = {50,20,50,255};
                    drawTextureColumn(
                        i, startY, deltaY, depth,
                        mapTile - 1, 1.0, 
                        texCoord, colorGrad, fadeColor
                    );
                    // Check for texture column transparency
                    bool hasAlpha = false;
                    for (uint32_t p = 0; p < tex_tile_height; p++)
                    {
                        if (( m_pixels[(mapTile-1)*tex_tile_width*tex_tile_height+texCoord+(tex_tile_width*p)] & 0xFF) < 0xFF)
                        {
                            constexpr int border = 2;
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
                            hasAlpha = true;
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
            constexpr int map_border = 2;
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
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, nullptr, 0);
    if (!rgbaData)
    {
        SPDLOG_ERROR("Could not load texture '{}'", path);
        return;
    }

    Pixelator* pixelator = m_pixelator.get();
    pixelator->addBuffer("raytex");
    pixelator->setSize("raytex", Vector2i(mPixWidth, mPixHeight));
    const std::string buf = pixelator->getActiveBuffer();
    pixelator->setActiveBuffer("raytex");
    pixelator->copy(rgbaData, Vector2i(mPixWidth, mPixHeight), 0, 0, IntRect(0, 0, mPixWidth, mPixHeight));
    m_pixels.resize(mPixWidth * mPixHeight * 4);
    memcpy(&m_pixels[0], pixelator->getPixelsPtr(), mPixWidth * mPixHeight * 4);
    pixelator->setActiveBuffer(buf);
    pixelator->removeBuffer("raytex");

    stbi_image_free(rgbaData);
}

void RayCaster::initWallCeilTexture(const std::string& path, int tile_width, int tile_height, int num_tiles)
{
    int32_t mPixWidth;
    int32_t mPixHeight;
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, nullptr, 0);
    if (!rgbaData)
    {
        SPDLOG_ERROR("Could not load texture '{}'", path);
        return;
    }

    Pixelator* pixelator = m_pixelator.get();
    pixelator->addBuffer("ceiltex");
    pixelator->setSize("ceiltex", Vector2i(mPixWidth, mPixHeight));
    const std::string buf = pixelator->getActiveBuffer();
    pixelator->setActiveBuffer("ceiltex");
    pixelator->copy(rgbaData, Vector2i(mPixWidth, mPixHeight), 0, 0, IntRect(0, 0, mPixWidth, mPixHeight));
    m_wall_ceil_pixels.resize(mPixWidth * mPixHeight * 4);
    memcpy(&m_wall_ceil_pixels[0], pixelator->getPixelsPtr(), mPixWidth * mPixHeight * 4);
    pixelator->setActiveBuffer(buf);
    pixelator->removeBuffer("ceiltex");

    stbi_image_free(rgbaData);
}

void RayCaster::renderFloor(const Camera* camera, double resolution) const
{
    const double scaleFactor = static_cast<double>(m_width) / static_cast<double>(m_height) * 2.4;

    // Get initial coordinate position at top-left of floor space
    constexpr uint32_t startX = 0;
    const uint32_t startY = m_height / 2;

    const double startAngle = camera->angle - camera->fov / 2.0;

    const ALLEGRO_COLOR fadeColor = al_map_rgba(50,20,50,255);
    
    // iterate through *all* pixels...
    for (int x = startX; x < m_width; x++)
    {
        // Establish angle of column...
        const double rayAngle = startAngle + camera->angleValues[x];
        const double rayCos = cos(rayAngle - camera->angle);

        for (int y = startY + 1; y < m_height; y++)
        {
            // Compute the distance to the pixel...
            const double pixelDist = static_cast<double>(m_height) * (1 + 2 * camera->h) / (10.0 * (y - startY - 1) * rayCos) * scaleFactor;
            constexpr double fogConstant = 4.0/5;
            //double pixelDepth = (pixelDist * rayCos);
            const double fadePercent = pixelDist / (camera->dist * fogConstant);
            const double pixelX = camera->x + pixelDist * cos(rayAngle);
            const double pixelY = camera->y + pixelDist * sin(rayAngle);
            // Wow, is that really it? The math says so...
            int r;
            int g;
            int b;
            // r = 74;
            // g = 82;
            // b = 99;
            if (pixelDist < camera->dist * fogConstant)
            {
                constexpr int tileNum = 0;
                // Get associated coordinate pixel...
                // TODO: some grid code...
                const uint32_t texX = static_cast<uint32_t>(floor(static_cast<double>(ceil_tile_width) * (pixelX - floor(pixelX))));
                const uint32_t texY = static_cast<uint32_t>(floor(static_cast<double>(ceil_tile_height) * (pixelY - floor(pixelY))));
                const uint32_t pixColor = m_wall_ceil_pixels[tileNum * ceil_tile_width * ceil_tile_height + texX + texY * ceil_tile_width];
                r = static_cast<int>(pixColor >> 3 * 8);
                g = static_cast<int>((pixColor >> 2 * 8) & 0xFF);
                b = static_cast<int>((pixColor >> 8) & 0xFF);
                const int dr = fadeColor.r - r;
                const int dg = fadeColor.g - g;
                const int db = fadeColor.b - b;
                r += static_cast<int>(static_cast<double>(dr) * fadePercent);
                g += static_cast<int>(static_cast<double>(dg) * fadePercent);
                b += static_cast<int>(static_cast<double>(db) * fadePercent);
            }
            else
            {
                r = fadeColor.r;
                g = fadeColor.g;
                b = fadeColor.b;
            }
            // buffer->pixels[x + y * buffer->width] = ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)0xFF);
            m_pixelator.get()->setPixel(x, y, al_map_rgba(r, g, b, 255));
            // buffer->pixels[x + y * buffer->width] = ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)0xFF);
        }
    }
}

void RayCaster::renderCeiling(const Camera* camera, double resolution) const
{
    const double scaleFactor = static_cast<double>(m_width) / static_cast<double>(m_height) * 2.4;

    // Get initial coordinate position at top-left of floor space
    constexpr uint32_t startX = 0;

    //double pixelDepth;

    const double startAngle = camera->angle - camera->fov / 2.0;

    // iterate through *all* pixels...
    for (int x = startX; x < m_width; x++)
    {
        constexpr uint32_t startY = 0;
        // Establish angle of column...
        const double rayAngle = startAngle + camera->angleValues[x];
        const double rayCos = cos(rayAngle - camera->angle);

        for (int y = startY; y < (m_height / 2)+1; y++)
        {
            constexpr ALLEGRO_COLOR fadeColor = {50,20,50,255};
            // Compute the distance to the pixel...
            const double pixelDist = static_cast<double>(m_height) * (1 - 2 * camera->h) / (10.0 * (m_height / 2 - y) * rayCos) * scaleFactor;
            //pixelDepth = (pixelDist * rayCos);
            constexpr double fogConstant = 4.0/5;
            const double fadePercent = pixelDist / (camera->dist * fogConstant);
            const double pixelX = camera->x + pixelDist * cos(rayAngle);
            const double pixelY = camera->y + pixelDist * sin(rayAngle);
            // Wow, is that really it? The math says so...
            int r;
            int g;
            int b;
            // r = 110;
            // g = 118;
            // b = 135;
            if (pixelDist < camera->dist * fogConstant)
            {
                constexpr int tileNum = 1;
                // Get associated coordinate pixel...
                // TODO: some grid code...
                const uint32_t texX = static_cast<uint32_t>(floor(static_cast<double>(ceil_tile_width) * (pixelX - floor(pixelX))));
                const uint32_t texY = static_cast<uint32_t>(floor(static_cast<double>(ceil_tile_height) * (pixelY - floor(pixelY))));
                const uint32_t pixColor = m_wall_ceil_pixels[tileNum * ceil_tile_width * ceil_tile_height + texX + texY * ceil_tile_width];
                r = static_cast<int>(pixColor >> 3 * 8);
                g = static_cast<int>((pixColor >> 2 * 8) & 0xFF);
                b = static_cast<int>((pixColor >> 8) & 0xFF);
                const int dr = fadeColor.r - r;
                const int dg = fadeColor.g - g;
                const int db = fadeColor.b - b;
                r += static_cast<int>(static_cast<double>(dr) * fadePercent);
                g += static_cast<int>(static_cast<double>(dg) * fadePercent);
                b += static_cast<int>(static_cast<double>(db) * fadePercent);
            }
            else
            {
                r = fadeColor.r;
                g = fadeColor.g;
                b = fadeColor.b;
            }
            // PixelRenderer::drawPix(buffer, x, y, PixelRenderer::toPixColor(r,g,b,0xff));
            m_pixelator.get()->setPixel(x, y, al_map_rgba(r, g, b, 255));
        }
    }
}
