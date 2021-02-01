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
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <ctime>
#include <cstdio>

#include "toml.hpp"
#include "utils.hpp"
#include "physfs.hpp"
#include "raylib-cpp.hpp"

// Custom logging funtion
void LogCustom(int msgType, const char *text, va_list args)
{
    FILE* logFile;
    logFile = fopen("log/pxllog.txt","a");

    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(logFile, "[%s] ", timeStr);

    switch (msgType)
    {
        case LOG_INFO: fputs("[INFO] : ", logFile); break;
        case LOG_ERROR: fputs("[ERROR]: ", logFile); break;
        case LOG_WARNING: fputs("[WARN] : ", logFile); break;
        case LOG_DEBUG: fputs("[DEBUG]: ", logFile); break;
        default: break;
    }

    vfprintf(logFile,text, args);
    fputs("\n",logFile);
    fclose(logFile);
}

void setup_working_directory()
{
    // Get executable path
    std::string path = std::filesystem::current_path().generic_string();
    // Remove the build directory, so that we land on appropriate directory for asset loading
    std::vector<std::string> strList;
    strList.push_back("/build/code/");
    strList.push_back("\\build\\code\\");
    strList.push_back("\\vsbuild\\code\\");
    strList.push_back("Release");
    strList.push_back("Debug");
    utility::eraseSubStrings(path, strList);
    // Set a proper working directory
    std::filesystem::current_path(path);

    std::string  message = "Current working directory is now " + path;

    TraceLog(LOG_INFO, message.c_str());
    std::cout << path << std::endl;
}

int main(void)
{
    setup_working_directory();

    // Remove old log file
    if(std::filesystem::exists("log/pxllog.txt"))
    {
        std::remove("log/pxllog.txt");
    }

    SetTraceLogCallback(LogCustom);

    auto config = toml::parse("assets/config/pxlwolf.toml");
    const auto& application_config = toml::find(config, "application");
    toml::table config_table = toml::get<toml::table>(application_config);

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = config_table["window_width"].as_integer();
    const int screenHeight = config_table["window_height"].as_integer();
    const float scale = config_table["window_scale"].as_floating();
    const std::string title = config_table["title"].as_string();

    raylib::Window window(screenWidth * static_cast<int>(scale), screenHeight * static_cast<int>(scale), title.c_str());

    raylib::AudioDevice audiodevice;
    raylib::Music music("assets/music/mini1111.xm");
    music.Play();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        music.Update();      // Update music buffer with new stream data
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    return 0;
}
