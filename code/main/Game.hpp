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
#pragma once

#include "Application.hpp"
#include "Map.hpp"
#include "RayCaster.hpp"

struct GameConfig
{
    std::string level_name;
    double camera_dist;
};

struct Camera
{
    double x;
    double y;
    double z;
    double h;
    double angle;
    double dirX;
    double dirY;
    double dirZ;
    double planeX;
    double planeY;
    double pitch;
    double fov;
    double dist;
    double angleValues[1024];
};

struct Texture
{
    std::vector<uint32_t> pixels;
    uint32_t tile_width;
    uint32_t tile_height;
    uint8_t tile_count;
};

struct Sprite
{
    std::string type_name;
    Texture texture;
    uint8_t frameNum;
    double alphaNum;
    double scaleFactor;
    double x;
    double y;
    double h;
};

struct Player
{
    double x;
    double y;
    double h;
    double groundH;
    double velX;
    double velY;
    double velH;
    double angle;
    uint8_t usingMouse;
    uint8_t health;
    uint8_t state;
    double timer;
    uint8_t spacePressed;
    Camera camera;
};

class Game : public Application
{
public:
    Game();
    ~Game();
    void setConfig(GameConfig config) { m_config = config; }

protected:
    bool OnUserCreate() override;
    bool OnUserUpdate(double deltaTime) override;
    bool OnUserRender() override;
    bool OnUserDestroy() override;

private:
    GameConfig m_config;
    std::shared_ptr<utility::Map> m_map;
    Camera m_camera;
    RayCaster m_raycaster;

    std::vector<Sprite> m_sprites;
    std::vector<Sprite> m_enemies;
    std::vector<Sprite> m_keys;
    int m_sprites_rendered;

    void setupCameraVectors();
    void handle_input(double deltaTime);

    void loadSprites();
    void addStatic(const std::string& type, int x, int y);
    void addPickup(const std::string& type, int x, int y);
    void addEnemy(const std::string& type, int x, int y);
    void addKey(const std::string& type, int x, int y);

    bool initSpriteTexture(Texture* texture, const std::string& path, int tile_width, int tile_height, int num_tiles);
    void initSprite(const std::string& type_name, Sprite* newSprite, Texture texture, double scaleFactor, double alphaNum, double x, double y, double h);
    void draw3DSprite(const std::string& buffer, Camera* camera, uint32_t width, uint32_t height, double resolution, Sprite sprite);
};
