/*
*   This file is part of the
* ███╗   ██╗ █████╗ ███████╗██╗     
* ████╗  ██║██╔══██╗██╔════╝██║     
* ██╔██╗ ██║███████║███████╗██║     
* ██║╚██╗██║██╔══██║╚════██║██║     
* ██║ ╚████║██║  ██║███████║███████╗
* ╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*   project : https://github.com/jacmoe/nasl
*
*   Copyright 2017 Jacob Moen
*
*/
#include "nasl_graphics.h"
#include <stdlib.h>
#include <stdio.h>

static GLFWwindow *window = NULL;

static void error_callback(int e, const char *d)
{
    printf("Error %d: %s\n", e, d);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


GLFWwindow* nasl_graphics_get_window()
{
    return window;
}

static char *ReadFile(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Coudn't read file: '%s'\n", path);
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(fsize + 1);
    fread(buf, fsize, 1, f);
    buf[fsize] = '\0';

    fclose(f);

    return buf;
}


static GLuint CompileShaderProgram() {
    char *vertexSource = ReadFile("assets/shaders/vertex.glsl");
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar **)&vertexSource, NULL);
    glCompileShader(vertexShader);
    free(vertexSource);

    char *fragmentSource = ReadFile("assets/shaders/frag.glsl");
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **)&fragmentSource, NULL);
    glCompileShader(fragmentShader);
    free(fragmentSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


static GLuint BuildQuad(GLuint shaderProgram) {
    GLfloat vertices[] = {
        // Vertex coord  Texture coord
        1,  1,           1, 0,
        -1,  1,          0, 0,
        1, -1,           1, 1,
        -1, -1,          0, 1
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex attributes
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
            0);
    glEnableVertexAttribArray(posAttrib);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
            (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(texAttrib);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vao;
}


static GLuint BuildTexture(int width, int height) {
    GLuint tex;

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}


static void InitOpenGL(int width, int height) {
    GLuint shaderProgram = CompileShaderProgram();
    GLuint quad_vao = BuildQuad(shaderProgram);
    GLuint tex = BuildTexture(width, height);

    glUseProgram(shaderProgram);
    glBindVertexArray(quad_vao);
    glBindTexture(GL_TEXTURE_2D, tex);
}

int nasl_graphics_init(int width, int height, const char* title, int fullscreen, int scalefactor)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create window and OpenGL context
    window = NULL;
    if(fullscreen)
    {
        window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
    } else {
        window = glfwCreateWindow(width, height, title, NULL, NULL);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!fullscreen) glfwSetWindowSize(window, width * scalefactor, height * scalefactor);
    
    // Position window in the middle of the screen
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xpos = (mode->width / 2) - (width * scalefactor) / 2;
    int ypos = (mode->height / 2) - (height * scalefactor) / 2;
    glfwSetWindowPos(window, xpos, ypos);

    glfwMakeContextCurrent(window);

    if(fullscreen)
    {
        glViewport(0, 0, mode->width, mode->height);
    } else {
        glViewport(0, 0, width, height);
    }

    // Init GLEW
    glewExperimental = GL_TRUE;
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    glewInit();

    if (GLEW_VERSION_4_3)
    {
        printf("GLEW found OPENGL 4.3\n");
    }

	// get version info
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
				 glGetString(GL_SHADING_LANGUAGE_VERSION));

    InitOpenGL(width, height);
   return 0;
}

int nasl_graphics_shutdown()
{
    glfwTerminate();
    return 0;
}

int nasl_graphics_running()
{
    return !glfwWindowShouldClose(window);
}

static uint32_t get_time()
{
    return 1000 * glfwGetTime();
}

uint32_t nasl_graphics_render(Buffer *buf)
{
    uint32_t start = get_time();

    glTexSubImage2D(
            GL_TEXTURE_2D,              // target
            0,                          // level of detail
            0, 0,                       // xoffset, yoffset
            buf->width, buf->height,    // width, height
            GL_RGBA, GL_UNSIGNED_BYTE,  // format, type
            buf->pixels                 // pixels
            );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    return get_time() - start;
}


void nasl_graphics_present()
{
    glfwSwapBuffers(window);
}

void nasl_graphics_poll_events()
{
    glfwPollEvents();
}
