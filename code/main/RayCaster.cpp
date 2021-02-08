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
#include "SDL.h"

SDL_Color FOG_COLOR = {50,20,50,255};

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

    m_atlas.load("assets/textures/wolfsheet.png", {128, 128});
    m_wall_ceil_atlas.load("assets/textures/ceilfloor.png", {512, 512});
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
    p_x = std::abs(p_x - map->width());

    /* Draw map tiles */
    for(row = 0; row < map->height(); row++)
    {
        for(col = 0; col < map->width(); col++)
        {
            blockRect.x = mapRect.x + col * blockSize;
            blockRect.y = mapRect.y + row * blockSize;
            if(map->minimap_walls()[row * map->width() + col] > 0)
            {
                uint32_t blockcolor = map->wall_element(map->minimap_walls()[row * map->width() + col]).color;
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

uint32_t RayCaster::pixelGradientShader(uint32_t pixel, double amount, SDL_Color target)
{
    int r = (int)(pixel >> 3*8);
    int g = (int)((pixel >> 2*8) & 0xFF);
    int b = (int)((pixel >> 8) & 0xFF);
    int a = (int)(pixel & 0xFF);
    // int dr = target.r - r;
    // int dg = target.g - g;
    // int db = target.b - b;
    // int da = target.a - a;
    // r += (int)((double)dr * amount);
    // g += (int)((double)dg * amount);
    // b += (int)((double)db * amount);
    // a += (int)((double)da * amount);
    int dr = (int)(r - amount);
    int dg = (int)(g - amount);
    int db = (int)(b - amount);
    int da = (int)(a - amount);
    return m_pixelator.get()->toIntColor(r,g,b,a);
}

void RayCaster::raycast(const Camera& camera)
{
    double dirX = camera.dirX;
    double dirY = camera.dirY;
    double planeX = camera.planeX;
    double planeY = camera.planeY;
    double posX = camera.x;
    double posY = camera.y;
    double pitch = camera.pitch; // looking up/down, expressed in screen pixels the horizon shifts
    double posZ = camera.z; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts
    // WALL CASTING
    for(int x = 0; x < m_width; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * x / double(m_width) - 1; //x-coordinate in camera space
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        //which box of the map we're in
        int mapX = int(posX);
        int mapY = int(posY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = std::abs(1 / rayDirX);
        double deltaDistY = std::abs(1 / rayDirY);
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if(rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if(rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if(sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //Check if ray has hit a wall
            if( m_map.get()->get_wall_entry(mapX, mapY) > 0 ) hit = 1;
        }

        //Calculate distance of perpendicular ray (Euclidean distance will give fisheye effect!)
        if(side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
        else          perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

        //Calculate height of line to draw on screen
        int lineHeight = (int)(m_height / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + m_height / 2 + pitch + (posZ / perpWallDist);
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + m_height / 2 + pitch + (posZ / perpWallDist);
        if(drawEnd >= m_height) drawEnd = m_height - 1;

        //texturing calculations
        int texNum = m_map.get()->get_wall_entry(mapX, mapY) - 1;

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if(side == 0) wallX = posY + perpWallDist * rayDirY;
        else          wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = int(wallX * double(m_atlas.getTileSize().x));
        if(side == 0 && rayDirX > 0) texX = m_atlas.getTileSize().x - texX - 1;
        if(side == 1 && rayDirY < 0) texX = m_atlas.getTileSize().x - texX - 1;

        // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * m_atlas.getTileSize().y / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - pitch - (posZ / perpWallDist) - m_height / 2 + lineHeight / 2) * step;
        for(int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (m_atlas.getTileSize().y - 1) in case of overflow
            int texY = (int)texPos & ((int)(m_atlas.getTileSize().y) - 1);
            texPos += step;

            uint32_t color = m_atlas.getPixel(texNum, texX, texY);
            color = pixelGradientShader(color, perpWallDist / 154 * 1.5/5, FOG_COLOR);
            m_pixelator->setPixel(x, y, color);
        }

        //SET THE ZBUFFER FOR THE SPRITE CASTING
        //m_depth_buffer[x] = perpWallDist;
        //   ZBuffer[x] = perpWallDist; //perpendicular distance is used
    }
}

void RayCaster::raycastCeilingFloor(const Camera& camera)
{
    double dirX = camera.dirX;
    double dirY = camera.dirY;
    double planeX = camera.planeX;
    double planeY = camera.planeY;
    double posX = camera.x;
    double posY = camera.y;
    double pitch = camera.pitch; // looking up/down, expressed in screen pixels the horizon shifts
    double posZ = camera.z; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts
    //FLOOR CASTING
    for(int y = 0; y < m_height; ++y)
    {
      // whether this section is floor or ceiling
      bool is_floor = y > m_height / 2 + pitch;

      // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      float rayDirX0 = dirX - planeX;
      float rayDirY0 = dirY - planeY;
      float rayDirX1 = dirX + planeX;
      float rayDirY1 = dirY + planeY;

      // Current y position compared to the center of the screen (the horizon)
      int p = is_floor ? (y - m_height / 2 - pitch) : (m_height / 2 - y + pitch);

      // Vertical position of the camera.
      // NOTE: with 0.5, it's exactly in the center between floor and ceiling,
      // matching also how the walls are being raycasted. For different values
      // than 0.5, a separate loop must be done for ceiling and floor since
      // they're no longer symmetrical.
      float camZ = is_floor ? (0.5 * m_height + posZ) : (0.5 * m_height - posZ);

      // Horizontal distance from the camera to the floor for the current row.
      // 0.5 is the z position exactly in the middle between floor and ceiling.
      // NOTE: this is affine texture mapping, which is not perspective correct
      // except for perfectly horizontal and vertical surfaces like the floor.
      // NOTE: this formula is explained as follows: The camera ray goes through
      // the following two points: the camera itself, which is at a certain
      // height (posZ), and a point in front of the camera (through an imagined
      // vertical plane containing the screen pixels) with horizontal distance
      // 1 from the camera, and vertical position p lower than posZ (posZ - p). When going
      // through that point, the line has vertically traveled by p units and
      // horizontally by 1 unit. To hit the floor, it instead needs to travel by
      // posZ units. It will travel the same ratio horizontally. The ratio was
      // 1 / p for going through the camera plane, so to go posZ times farther
      // to reach the floor, we get that the total horizontal distance is posZ / p.
      float rowDistance = camZ / p;

      // calculate the real world step vector we have to add for each x (parallel to camera plane)
      // adding step by step avoids multiplications with a weight in the inner loop
      float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / m_width;
      float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / m_width;

      // real world coordinates of the leftmost column. This will be updated as we step to the right.
      float floorX = posX + rowDistance * rayDirX0;
      float floorY = posY + rowDistance * rayDirY0;

      for(int x = 0; x < m_width; ++x)
      {
        // the cell coord is simply got from the integer parts of floorX and floorY
        int cellX = (int)(floorX);
        int cellY = (int)(floorY);

        // get the texture coordinate from the fractional part
        int tx = (int)(m_atlas.getTileSize().x * (floorX - cellX)) & ((int)(m_atlas.getTileSize().x) - 1);
        int ty = (int)(m_atlas.getTileSize().y * (floorY - cellY)) & ((int)(m_atlas.getTileSize().y) - 1);

        floorX += floorStepX;
        floorY += floorStepY;

        // choose texture and draw the pixel
        int checkerBoardPattern = (int(cellX + cellY)) & 1;
        int floorTexture;
        if(checkerBoardPattern == 0) floorTexture = 0;
        else floorTexture = 0;
        int ceilingTexture = 1;
        uint32_t color;

        if(is_floor) {
            // floor
            color = m_wall_ceil_atlas.getPixel(floorTexture, ty, tx);
            color = pixelGradientShader(color, rowDistance / 154 * 1.5/5, FOG_COLOR);
            // color = ColorAlphaBlend(color, {(unsigned char)(color.r / rowDistance),(unsigned char)(color.g / rowDistance),(unsigned char)(color.b / rowDistance), (unsigned char)(color.a / rowDistance)}, GOLD);
            m_pixelator->setPixel(x, y, color);
        } else {
            //ceiling
            color = m_wall_ceil_atlas.getPixel(ceilingTexture, ty, tx);
            color = pixelGradientShader(color, rowDistance / 154 * 1.5/5, FOG_COLOR);
            // color = ColorAlphaBlend(color, {(unsigned char)(color.r / rowDistance),(unsigned char)(color.g / rowDistance),(unsigned char)(color.b / rowDistance), (unsigned char)(color.a / rowDistance)}, GOLD);
            m_pixelator->setPixel(x, y, color);
        }
      }
    }
}