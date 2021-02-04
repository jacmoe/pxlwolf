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
#include "raymath.h"

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

    m_atlas.load("assets/textures/spritesheet.png", {512, 512});
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

Color RayCaster::pixelGradientShader(Color pixel, unsigned char amount)
{
    Color color;
    color.r = pixel.r - amount;
    color.g = pixel.g - amount;
    color.b = pixel.b - amount;
    color.a = pixel.a;
    return color;
}

void RayCaster::raycast(const _Camera& camera)
{
    double dirX = camera.dirX;
    double dirY = camera.dirY;
    double planeX = camera.planeX;
    double planeY = camera.planeY;
    double posX = camera.x;
    double posY = camera.y;
    double pitch = 0; // looking up/down, expressed in screen pixels the horizon shifts
    double posZ = 0; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts
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

        int texWidth = m_atlas.getTileSize().x;
        int texHeight =m_atlas.getTileSize().y;

        //x coordinate on the texture
        int texX = int(wallX * double(texWidth));
        if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
        if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

        // TODO: an integer-only bresenham or DDA like algorithm could make the texture coordinate stepping faster
        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texHeight / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - pitch - (posZ / perpWallDist) - m_height / 2 + lineHeight / 2) * step;
        for(int y = drawStart; y < drawEnd; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texHeight - 1);
            texPos += step;

            Color color = m_atlas.getPixel(texNum, texX, texY);

            //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
            // if(side == 1)
            // {
            //     int tinted_color = ColorToInt(color);
            //     tinted_color = (tinted_color >> 1) & 8355711;
            //     color = GetColor(tinted_color);
            // }
            m_pixelator->setPixel(x, y, color);
        }

        //SET THE ZBUFFER FOR THE SPRITE CASTING
        //m_depth_buffer[x] = perpWallDist;
        //   ZBuffer[x] = perpWallDist; //perpendicular distance is used
    }
}