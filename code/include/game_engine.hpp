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
#   MIT License
#*/
#pragma once

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "raycaster_engine.hpp"

typedef struct _Player {
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
} Player;

enum KeyMapNames {
    PK_FORWARD,
    PK_BACKWARD,
    PK_LSTRAFE,
    PK_RSTRAFE,
    PK_TC,
    PK_TCC,
    PK_JUMP,
    PK_CROUCH,
    PK_SPRINT,
    PK_PAUSE,
    PK_KILL,
    PK_RESPAWN,
    PK_USE,
    PK_INTERACT,
    LEN_PK,
    TERMINATE_PK
};

typedef struct _KeyPair {
    uint8_t primary;
    uint8_t secondary;
} KeyPair;

typedef struct _KeyMap {
    KeyPair keys[LEN_PK];
    uint8_t state[LEN_PK];
} KeyMap;

typedef struct _Entity {
    double x;
    double y;
    double h;
    double angle;
    uint8_t health;
    uint8_t state;
	RaySprite shadow;
	RaySprite sprite;
} Entity;

typedef struct _ProjectileList {
    Entity projectiles[64];
} ProjectileList;

class GameEngine
{
public:
    static void initPlayer(Player* player, double x, double y, double angle, uint8_t usingMouse, double fov, double viewDist, uint32_t screenWidth);
    static void initEntity(Entity* entity, double x, double y, double h, double angle, RayTex* spriteTex, RayTex* shadowTex);
    static void initProjectiles(ProjectileList* projectiles, uint32_t numProjectile, RayTex* spriteTex, RayTex* shadowTex);
    static void moveEntity(Entity* entity, double x, double y, double h);
    static void scaleEntity(Entity* entity, double scaleFactor);
    static void updatePlayer(Player* player, Map* map, KeyMap* keyMap, double dt);
    static void updateEntity(Entity* entity);
    static void updateProjectile(ProjectileList* projectiles, uint32_t numProjectile, Player* player);
    static void bindKeys(KeyMap* keyMap, uint8_t* keyList);
    static void updateKeys(KeyMap* keyMap);
};
