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
#   (c) 2020 Jacob Moena
#
#   BSD 2-Clause "Simplified" License
#*/
#include <iostream>
#include <filesystem>
#include <SDL.h>
#include "dbg_console.hpp"
#include "utils.hpp"
#include "system.hpp"
#include "buffer.hpp"
#include "draw.hpp"
#include "color.hpp"

static int init(int width, int height);
static int shutdown();

char maze[16][16] = {
	{1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 0,    1, 1, 1, 0,    0, 0, 0, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 1, 0,    1, 1, 1, 0,    1, 1, 0, 1},

	{1, 0, 1, 0,    1, 0, 1, 0,    0, 0, 1, 0,    0, 1, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 0, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    1, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 1, 1, 0,    1, 1, 0, 1},

	{1, 0, 1, 0,    0, 0, 1, 0,    0, 0, 1, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 1, 0,    1, 1, 0, 1},
	{1, 0, 1, 0,    1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 0,    1, 0, 1, 1,    1, 1, 0, 1},

	{1, 0, 0, 0,    1, 0, 1, 0,    1, 0, 0, 0,    0, 1, 0, 1},
	{1, 0, 1, 1,    1, 0, 1, 1,    1, 1, 1, 1,    1, 1, 0, 1},
	{1, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 0,    0, 0, 0, 1},
	{1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1,    1, 1, 1, 1}
};

typedef struct xy {
	int x, y;
};

struct xy increment[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
struct xy left[4] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
struct xy right[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
struct xy pos = {1,3};

int direction = 3;
int visibility = 4;

void draw_maze(Buffer* buffer);
void draw_box(Buffer* buffer);

int main(int, char**){
    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 
    CreateConsoleWindow();
    std::cout << "In main!" << std::endl;
    std::cout << path << std::endl;
    std::cout << std::filesystem::current_path().string() << std::endl;

    int buffer_width = 320;
    int buffer_height = 240;

    init(buffer_width, buffer_height);

    // Create main buffer
    Buffer* buffer = B_CreateBuffer(buffer_width, buffer_height);
    //nasl_buffer_set_mainbuffer(buffer);
    // Clear main buffer to a dark grey color
    //nasl_buffer_clear(buffer, GREY1);

    Buffer* background = S_LoadImage("assets/textures/background.png");
    B_BlitBuffer(buffer, background, 0, 0);

    draw_box(buffer);
    draw_maze(buffer);

    // Main loop
    while(true)
    {
        // Event polling
        S_GetTick();
        //nasl_graphics_poll_events();
        // Render the main buffer
        S_Blit(buffer);
        // Swap buffers
        //nasl_graphics_present();
    }

    // Destroy the main buffer
    B_DeleteBuffer(buffer);

    shutdown();

    return 0;
}

// Draw the maze stored in array maze[] into video
// memory at address screen.
void draw_maze(Buffer* buffer)
{
	struct xy block, lblock, rblock;
	int oldleft, oldright;

	// Draw the maze at each distance allowed by visibility
	for(int dist = 0; dist < visibility; dist++)
	{
		// Find current square of maze
		block.x = pos.x + dist * increment[direction].x;
		block.y = pos.y + dist * increment[direction].y;

		// Find square to the left of current square
		lblock.x = block.x + left[direction].x;
		lblock.y = block.y + left[direction].y;

		// Find square to the right of current square
		rblock.x = block.x + right[direction].x;
		rblock.y = block.y + right[direction].y;

		// Draw image of squares according to distance
		switch(dist)
		{
			case 0:		// Draw current square

				// Is wall open to the left?
				// If not, draw wall
				if (maze[block.x][block.y])
				{
					D_DrawLine(buffer, 82, 19, 135, 44, Color::GREY);
					D_DrawLine(buffer, 135, 44, 135, 93, Color::GREY);
					D_DrawLine(buffer, 135, 93, 82, 118, Color::GREY);
				}
				else // Else draw opening
				{
					D_DrawLine(buffer, 82, 44, 135, 44, Color::GREY);
					D_DrawLine(buffer, 135, 44, 135, 93, Color::GREY);
					D_DrawLine(buffer, 135, 93, 82, 93, Color::GREY);
				}

				// Is wall open to the right?
				// If not, draw wall
				if (maze[rblock.x][rblock.y])
				{
					D_DrawLine(buffer, 294, 19, 242, 44, Color::GREY);
					D_DrawLine(buffer, 242, 44, 242, 93, Color::GREY);
					D_DrawLine(buffer, 294, 118, 242, 93, Color::GREY);
				}
				else	// Else draw opening
				{
					D_DrawLine(buffer, 294, 44, 242, 44, Color::GREY);
					D_DrawLine(buffer, 242, 44, 242, 93, Color::GREY);
					D_DrawLine(buffer, 242, 93, 294, 93, Color::GREY);
				}
				break;

			case 1:		// Repeat for next square

				// Can we see the next square?
				// If not, draw wall
				if(maze[block.x][block.y])
				{
					D_DrawLine(buffer, 135, 44, 135, 93, Color::GREY);
					D_DrawLine(buffer, 242, 44, 242, 93, Color::GREY);
					D_DrawLine(buffer, 135, 44, 242, 44, Color::GREY);
					D_DrawLine(buffer, 135, 93, 242, 93, Color::GREY);
				}
				else	// Else draw sides of the next square
				{
					if (maze[lblock.x][lblock.y])
					{
						D_DrawLine(buffer, 135, 44, 162, 57, Color::GREY);
						D_DrawLine(buffer, 162, 57, 162, 80, Color::GREY);
						D_DrawLine(buffer, 162, 80, 135, 93, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 135, 57, 162, 57, Color::GREY);
						D_DrawLine(buffer, 162, 57, 162, 80, Color::GREY);
						D_DrawLine(buffer, 162, 80, 135, 80, Color::GREY);
					}
					if (maze[rblock.x][rblock.y])
					{
						D_DrawLine(buffer, 242, 44, 215, 57, Color::GREY);
						D_DrawLine(buffer, 215, 57, 215, 80, Color::GREY);
						D_DrawLine(buffer, 215, 80, 242, 93, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 242, 57, 215, 57, Color::GREY);
						D_DrawLine(buffer, 215, 57, 215, 80, Color::GREY);
						D_DrawLine(buffer, 215, 80, 242, 80, Color::GREY);
					}
				}
				break;

			case 2:		// Do it again
				if (maze[block.x][block.y])
				{
					D_DrawLine(buffer, 162, 57, 162, 80, Color::GREY);
					D_DrawLine(buffer, 215, 57, 215, 80, Color::GREY);
					D_DrawLine(buffer, 162, 57, 215, 57, Color::GREY);
					D_DrawLine(buffer, 162, 80, 215, 80, Color::GREY);
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						D_DrawLine(buffer, 162, 57, 175, 63, Color::GREY);
						D_DrawLine(buffer, 175, 63, 175, 74, Color::GREY);
						D_DrawLine(buffer, 175, 74, 162, 80, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 162, 63, 175, 63, Color::GREY);
						D_DrawLine(buffer, 175, 63, 175, 74, Color::GREY);
						D_DrawLine(buffer, 175, 74, 162, 74, Color::GREY);
					}
					if (maze[rblock.x][rblock.y])
					{
						D_DrawLine(buffer, 215, 57, 202, 63, Color::GREY);
						D_DrawLine(buffer, 202, 63, 202, 74, Color::GREY);
						D_DrawLine(buffer, 202, 74, 215, 80, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 215, 63, 202, 63, Color::GREY);
						D_DrawLine(buffer, 202, 63, 202, 74, Color::GREY);
						D_DrawLine(buffer, 202, 74, 215, 74, Color::GREY);
					}
				}
				break;

			case 3:		// And again
				if (maze[block.x][block.y])
				{
					D_DrawLine(buffer, 175, 63, 175, 74, Color::GREY);
					D_DrawLine(buffer, 202, 63, 202, 74, Color::GREY);
					D_DrawLine(buffer, 175, 63, 202, 63, Color::GREY);
					D_DrawLine(buffer, 175, 74, 202, 74, Color::GREY);
				}
				else
				{
					if (maze[lblock.x][lblock.y])
					{
						D_DrawLine(buffer, 175, 63, 182, 66, Color::GREY);
						D_DrawLine(buffer, 182, 66, 182, 70, Color::GREY);
						D_DrawLine(buffer, 182, 70, 175, 74, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 175, 66, 182, 66, Color::GREY);
						D_DrawLine(buffer, 182, 66, 182, 70, Color::GREY);
						D_DrawLine(buffer, 182, 70, 175, 70, Color::GREY);
					}
					if (maze[rblock.x][rblock.y])
					{
						D_DrawLine(buffer, 202, 63, 195, 66, Color::GREY);
						D_DrawLine(buffer, 195, 66, 195, 70, Color::GREY);
						D_DrawLine(buffer, 195, 70, 202, 74, Color::GREY);
					}
					else
					{
						D_DrawLine(buffer, 202, 66, 195, 66, Color::GREY);
						D_DrawLine(buffer, 195, 66, 195, 70, Color::GREY);
						D_DrawLine(buffer, 195, 70, 202, 70, Color::GREY);
					}
				}
				break;
		}

		// If view is obscured by wall, stop drawing
		if(maze[block.x][block.y]) break;
	}
}

void draw_box(Buffer* buffer)
{
	//left, top, right, bottom
	//nasl_draw_rect(buffer, 82, 19, 294, 119, Color::GREY);
}

static int init(int width, int height)
{
    S_Init("Neogardens Wireframe Maze Demo", width, height);

    //glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);

    return 1;
}

//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
//}

static int shutdown()
{
    S_Quit();

    return 1;
}
