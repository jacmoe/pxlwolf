/*# This file is part of the
  #   ___ _       __    __      _  __ 
  #  / _ \ |_  __/ / /\ \ \___ | |/ _|
  # / /_)/ \ \/ /\ \/  \/ / _ \| | |_ 
  #/ ___/| |>  <  \  /\  / (_) | |  _|
  #\/    |_/_/\_\  \/  \/ \___/|_|_|  
  # project
  # https://github.com/jacmoe/pxlwolf
  #
  # (c) 2020 Jacob Moena
  #
  # BSD 2-Clause "Simplified" License
  #
*/
#ifdef _WIN32
#include <windows.h>
#endif
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "nasl_graphics.h"
#include "nasl_buffer.h"
#include "nasl_image.h"
#include "nasl_draw.h"
#include "nasl_sprite.h"
#include "nasl_geometry.h"
#include "utils.h"

#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight] =
    {
        {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 6, 4, 4, 6, 4, 6, 4, 4, 4, 6, 4},
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
        {8, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6},
        {8, 0, 3, 3, 0, 0, 0, 0, 0, 8, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 4, 0, 0, 0, 0, 0, 6, 6, 6, 0, 6, 4, 6},
        {8, 8, 8, 8, 0, 8, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 4, 6, 0, 0, 0, 0, 0, 6},
        {7, 7, 7, 7, 0, 7, 7, 7, 7, 0, 8, 0, 8, 0, 8, 0, 8, 4, 0, 4, 0, 6, 0, 6},
        {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 0, 0, 0, 0, 0, 6},
        {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 0, 0, 0, 0, 4},
        {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 6, 0, 6, 0, 6, 0, 6},
        {7, 7, 0, 0, 0, 0, 0, 0, 7, 8, 0, 8, 0, 8, 0, 8, 8, 6, 4, 6, 0, 6, 6, 6},
        {7, 7, 7, 7, 0, 7, 7, 7, 7, 8, 8, 4, 0, 6, 8, 4, 8, 3, 3, 3, 0, 3, 3, 3},
        {2, 2, 2, 2, 0, 2, 2, 2, 2, 4, 6, 4, 0, 0, 6, 0, 6, 3, 0, 0, 0, 0, 0, 3},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 3},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 4, 4, 4, 4, 4, 6, 0, 6, 3, 3, 0, 0, 0, 3, 3},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 2, 2, 2, 6, 6, 0, 0, 5, 0, 5, 0, 5},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 5, 0, 5, 0, 5, 0, 5, 0, 5},
        {2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 0, 5, 0, 5, 0, 0, 0, 5, 5},
        {2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

double posX = 22.0, posY = 11.5;    //x and y start position
double dirX = -1.0, dirY = 0.0;     //initial direction vector
double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

double curr_time = 0;    //time of current frame
double oldTime = 0; //time of previous frame
double frameTime = 0;

int move_forwards = 0;
int move_backwards = 0;
int turn_left = 0;
int turn_right = 0;

int mouse_x = 0;
int mouse_y = 0;
int xdelta = 0;
int ydelta = 0;

static int init(int width, int height);
static int shutdown();
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void draw_maze(Buffer *buffer, SpriteSheet textures);

bool load_level(const std::string level_name);

#ifdef _WIN32f
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 

    load_level("assets/levels/level.ldtk");

    int buffer_width = 320;
    int buffer_height = 240;

    init(buffer_width, buffer_height);

    glfwSwapInterval(1);

    // Create main buffer
    Buffer *buffer = nasl_buffer_create(buffer_width, buffer_height);
    nasl_buffer_set_mainbuffer(buffer);

    // Load textures
    SpriteSheet textures = nasl_sprite_load("assets/textures/sjswalls2.bmp", 4, 3);

    // Load font
    SpriteSheet ascii = nasl_sprite_load("assets/fonts/ascii.png", 16, 16);

    int win_width, win_height;
    glfwGetWindowSize(nasl_graphics_get_window(), &win_width, &win_height);
    framebuffer_size_callback(nasl_graphics_get_window(), win_width, win_height);
    // Main loop
    while (nasl_graphics_running())
    {
        oldTime = curr_time;
        curr_time = glfwGetTime();
        frameTime = curr_time - oldTime; //frameTime is the time this frame has taken, in seconds

        //speed modifiers
        double moveSpeed = frameTime * 3.0; //the constant value is in squares/second
        double rotSpeed = frameTime * 1.0;   //the constant value is in radians/second

        ///*
        double delta = xdelta * (frameTime * 0.1);
        if(xdelta < 0)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(delta) - dirY * sin(delta);
            dirY = oldDirX * sin(delta) + dirY * cos(delta);
            double oldPlaneX = planeX;
            planeX = planeX * cos(delta) - planeY * sin(delta);
            planeY = oldPlaneX * sin(delta) + planeY * cos(delta);
            xdelta = 0.0;
        } else if(xdelta > 0) {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(delta) - dirY * sin(delta);
            dirY = oldDirX * sin(delta) + dirY * cos(delta);
            double oldPlaneX = planeX;
            planeX = planeX * cos(delta) - planeY * sin(delta);
            planeY = oldPlaneX * sin(delta) + planeY * cos(delta);
            xdelta = 0.0;
        }
        //*/
        if(move_forwards)
        {
            if (!worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)])
                posX += dirX * moveSpeed;
            if (!worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)])
                posY += dirY * moveSpeed;
        }
        if(move_backwards)
        {
            if (!worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)])
                posX -= dirX * moveSpeed;
            if (!worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)])
                posY -= dirY * moveSpeed;
        }
        if(turn_left)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
        }
        if(turn_right)
        {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
        }

        // Event polling
        nasl_graphics_poll_events();

        // Clear buffer
        nasl_buffer_clear(buffer, GREY1);
        
        // Draw maze
        draw_maze(buffer, textures);
        
        // Print instructions
        nasl_draw_text(buffer, 20, buffer->height - 14, Color::GREY1, 10, "WASD to move. ESC to quit");

        // Render the main buffer
        nasl_graphics_render(buffer);
        
        // Swap buffers
        nasl_graphics_present();

        // Center mouse cursor
        //glfwSetCursorPos(nasl_graphics_get_window(), win_width / 2, win_height / 2);
    }

    nasl_sprite_delete(textures);
    nasl_sprite_delete(ascii);
    // Destroy the main buffer
    nasl_buffer_destroy(buffer);

    shutdown();
}

static Buffer *get_texture(SpriteSheet textures, int idx)
{
    int col = idx % textures.cols;
    int row = idx / textures.cols;
    return nasl_sprite_get(textures, row, col);
}

void draw_maze(Buffer *buffer, SpriteSheet textures)
{
    for (int x = 0; x < buffer->width; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * x / (double)buffer->width - 1; //x-coordinate in camera space
        double rayPosX = posX;
        double rayPosY = posY;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        //which box of the map we're in
        int mapX = (int)rayPosX;
        int mapY = (int)rayPosY;

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        //length of ray from one x or y-side to next x or y-side
        double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side;    //was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, OR in x-direction, OR in y-direction
            if (sideDistX < sideDistY)
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
            if (worldMap[mapX][mapY] > 0)
                hit = 1;
        }

        //Calculate distance of perpendicular ray (oblique distance will give fisheye effect!)
        if (side == 0)
            perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

        //Calculate height of line to draw on screen
        int lineHeight = (int)(buffer->height / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + buffer->height / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + buffer->height / 2;
        if (drawEnd >= buffer->height)
            drawEnd = buffer->height - 1;
        //texturing calculations
        int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!
        Buffer* texture = get_texture(textures, texNum);

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if (side == 0)
            wallX = rayPosY + perpWallDist * rayDirY;
        else
            wallX = rayPosX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = (int)(wallX * (double)textures.width);
        if (side == 0 && rayDirX > 0)
            texX = textures.width - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = textures.width - texX - 1;
        for (int y = drawStart; y < drawEnd; y++)
        {
            int d = y * 256 - buffer->height * 128 + lineHeight * 128; //256 and 128 factors to avoid floats
            int texY = ((d * textures.height) / lineHeight) / 256;
            uint32_t color = nasl_buffer_get_pixel(texture, texX, texY);
                
            double scalefactor = 1.0 - ((perpWallDist / 576) * 100);
            if(scalefactor > 1.0) scalefactor = 1.0;
            if(scalefactor < 0.01) scalefactor = 0.01;
            if(perpWallDist > 0.1)
                color = nasl_color_scale(color, scalefactor);
            
            color = nasl_color_scale(color, 1.5);

            nasl_buffer_set_pixel(buffer, x, y, color);
        }

        //FLOOR CASTING
        double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

        //4 different wall directions possible
        if (side == 0 && rayDirX > 0)
        {
            floorXWall = mapX;
            floorYWall = mapY + wallX;
        }
        else if (side == 0 && rayDirX < 0)
        {
            floorXWall = mapX + 1.0;
            floorYWall = mapY + wallX;
        }
        else if (side == 1 && rayDirY > 0)
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY;
        }
        else
        {
            floorXWall = mapX + wallX;
            floorYWall = mapY + 1.0;
        }

        double distWall, distPlayer, currentDist;

        distWall = perpWallDist;
        distPlayer = 0.0;

        if (drawEnd < 0)
            drawEnd = buffer->height; //becomes < 0 when the integer overflows

        //draw the floor from drawEnd to the bottom of the screen
        for (int y = drawEnd; y < buffer->height; y++)
        {
            currentDist = buffer->height / (2.0 * y - buffer->height); //you could make a small lookup table for this instead

            double weight = (currentDist - distPlayer) / (distWall - distPlayer);

            double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
            double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

            int floorTexX, floorTexY;
            floorTexX = (int)(currentFloorX * textures.width) % textures.width;
            floorTexY = (int)(currentFloorY * textures.height) % textures.height;

            //floor
            texture = get_texture(textures, 5);
            uint32_t color = (texture->pixels[textures.width * floorTexY + floorTexX] >> 1) & 8355711;
            color = nasl_color_scale(color, 0.3);
            nasl_buffer_set_pixel(buffer, x, y, color);
            //ceiling (symmetrical!)
            texture = get_texture(textures, 10);
            color = texture->pixels[textures.width * floorTexY + floorTexX];
            color = nasl_color_scale(color, 0.3);
            nasl_buffer_set_pixel(buffer, x, buffer->height - y, color);
        }
    }
}

bool load_level(const std::string level_name)
{
    std::vector<int> wMap;
    std::vector<int> tMap;
    Vector player_pos;
    float player_heading;
    Vector world_size;

    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 

    FILE* fp = fopen("assets/levels/level.ldtk", "rb");

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document;
    document.ParseStream(is);

    const rapidjson::Value& levels = document["levels"];
    for (rapidjson::Value::ConstValueIterator itr = levels.Begin(); itr != levels.End(); ++itr)
    {
        const rapidjson::Value& layerInstances = (*itr)["layerInstances"];
        for (rapidjson::Value::ConstValueIterator itr = layerInstances.Begin(); itr != layerInstances.End(); ++itr)
        {
            std::string layer_type = (*itr)["__type"].GetString();
            if(layer_type == "Entities")
            {
                const rapidjson::Value& entityInstances = (*itr)["entityInstances"];
                for (rapidjson::Value::ConstValueIterator itr = entityInstances.Begin(); itr != entityInstances.End(); ++itr)
                {
                    std::string identifier = (*itr)["__identifier"].GetString();
                    if(identifier == "PlayerStart")
                    {
                        player_pos.x = (*itr)["__grid"].GetArray()[0].GetFloat();
                        player_pos.y = (*itr)["__grid"].GetArray()[1].GetFloat();
                        player_heading = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat();
                    }
                }
            }
            if(layer_type == "IntGrid")
            {
                world_size.x = (*itr)["__cWid"].GetInt();
                world_size.y = (*itr)["__cHei"].GetInt();
                for(int elems = 0; elems < world_size.x * world_size.y; elems++)
                {
                    wMap.push_back(-1);
                }

                const rapidjson::Value& initGrid = (*itr)["intGrid"];

                for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
                {
                    wMap[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt();
                }
            }

        }
    }
    fclose(fp);
    std::cout << "Parsed the following: " << std::endl;
    std::cout << "World size : " << world_size.x << "x" << world_size.y << std::endl;
    std::cout << "Player position : (" << player_pos.x << "," << player_pos.y << ")" << std::endl;
    std::cout << "Player heading : " << player_heading << std::endl;
}



static int init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Lightsourced Raycast Maze Demo", 0, 3);

    glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);
    //glfwSetCursorPosCallback(nasl_graphics_get_window(), cursor_position_callback);
    glfwSetInputMode(nasl_graphics_get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    return 1;
}

static void handle_keypress(int key, int action)
{
    // Do we want to move forward?
    if (key == GLFW_KEY_W)
    {
        if(action == GLFW_PRESS)
            move_forwards = 1;
        else if(action == GLFW_RELEASE)
            move_forwards = 0;
    }
    // or do we want to go backward?
    else if (key == GLFW_KEY_S)
    {
        if(action == GLFW_PRESS)
            move_backwards = 1;
        else if(action == GLFW_RELEASE)
            move_backwards = 0;
    }
    // Do we want to turn left?
    if (key == GLFW_KEY_A)
    {
        if(action == GLFW_PRESS)
            turn_left = 1;
        else if(action == GLFW_RELEASE)
            turn_left = 0;
    }
    // or do we want to turn right?
    else if (key == GLFW_KEY_D)
    {
        if(action == GLFW_PRESS)
            turn_right = 1;
        else if(action == GLFW_RELEASE)
            turn_right = 0;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    xdelta = mouse_x - xpos;
    ydelta = mouse_y - ypos;

    mouse_x = xpos;
    mouse_y = ypos;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else
    {
        handle_keypress(key, action);
    }
}

static int shutdown()
{
    nasl_graphics_shutdown();

    return 1;
}
