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
#include "Game.hpp"
#include "stb_image.h"
#include "unpacker.hpp"

Game::Game()
{
    m_map = std::make_shared<utility::Map>();
}

Game::~Game()
{
}

void Game::setupCameraVectors()
{
    m_camera.dirX = cos(m_camera.angle);
    m_camera.dirY = sin(m_camera.angle);
    double v_y = std::sin(m_camera.angle);
    double v_x = std::cos(m_camera.angle);
    linalg::aliases::double2 vect = {v_x, v_y};
    double a_y = std::sin(-90 * 4.0 * atan (1.0) / 180.0);
    double a_x = std::cos(-90 * 4.0 * atan (1.0) / 180.0);
    linalg::aliases::double2 vect_rotated =  {vect.x*a_x - vect.y*a_y, vect.x*a_y + vect.y*a_x};
    m_camera.planeX = vect_rotated.x;
    m_camera.planeY = vect_rotated.y * 0.66;
}

void Game::handle_input(double deltaTime)
{
    double delta_seconds = deltaTime * 0.001;
    double moveSpeed = delta_seconds * 3.0; //the constant value is in squares/second
    double rotSpeed = delta_seconds * 1.6; //the constant value is in radians/second

    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if( currentKeyStates[ SDL_SCANCODE_LSHIFT ] ) // run
    {
        moveSpeed *= 3.0;
    }

    utility::Map* map = m_map.get();

    if( currentKeyStates[ SDL_SCANCODE_R ] ) // respawn player at player start
    {
        m_camera.x = map->player_start().x;
        m_camera.y = map->player_start().y;
        m_camera.angle = map->player_heading();
        setupCameraVectors();
    }

    if( currentKeyStates[ SDL_SCANCODE_W ] ) // move forward
    {
        if(map->get_wall_entry(int(m_camera.x + m_camera.dirX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x += m_camera.dirX * moveSpeed;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y + m_camera.dirY * moveSpeed)) < 1)
        {
            m_camera.y += m_camera.dirY * moveSpeed;
        }
    }
    if( currentKeyStates[ SDL_SCANCODE_S ] ) // move backward
    {
        if(map->get_wall_entry(int(m_camera.x - m_camera.dirX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x -= m_camera.dirX * moveSpeed;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y - m_camera.dirY * moveSpeed)) < 1)
        {
            m_camera.y -= m_camera.dirY * moveSpeed;
        }
    }
    if( currentKeyStates[ SDL_SCANCODE_D ] ) // strafe right
    {
        if(map->get_wall_entry(int(m_camera.x - m_camera.planeX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x -= m_camera.planeX * moveSpeed * 1.6;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y - m_camera.planeY * moveSpeed)) < 1)
        {
            m_camera.y -= m_camera.planeY * moveSpeed * 1.6;
        }
    }
    if( currentKeyStates[ SDL_SCANCODE_A ] ) // strafe left
    {
        if(map->get_wall_entry(int(m_camera.x + m_camera.planeX * moveSpeed), int(m_camera.y)) < 1)
        {
            m_camera.x += m_camera.planeX * moveSpeed * 1,6;
        }
        if(map->get_wall_entry(int(m_camera.x), int(m_camera.y + m_camera.planeY * moveSpeed)) < 1)
        {
            m_camera.y += m_camera.planeY * moveSpeed * 1.6;
        }
    }

    if (SDL_GetRelativeMouseMode())
    {
        // Get mouse change since last frame
        int32_t mouseX;
        SDL_GetRelativeMouseState(&mouseX, NULL);
        // Update player
        m_camera.angle += 0.1 * rotSpeed * mouseX;
        setupCameraVectors();
    }
}

bool Game::OnUserCreate()
{
    utility::Map* map = m_map.get();
    map->init("assets/levels/pxlwolf.ldtk");
    map->load(m_config.level_name);

    write_text("PixelWolf");

    m_camera.x = m_map.get()->player_start().x;
    m_camera.y = m_map.get()->player_start().y;
    m_camera.angle = m_map.get()->player_heading();
    m_camera.h = 0;
    m_camera.dist = m_config.camera_dist;
    m_camera.fov = 3.14159 / 2;

    setupCameraVectors();

    m_camera.pitch = 0; // looking up/down, expressed in screen pixels the horizon shifts
    m_camera.z = 0; // vertical camera strafing up/down, for jumping/crouching. 0 means standard height. Expressed in screen pixels a wall at distance 1 shifts

    m_raycaster.init(m_width, m_height, m_map, m_pixelator);

    m_raycaster.generateAngleValues(m_width, &m_camera);

    m_pixelator.get()->setActiveBuffer("pixelBuffer");

    loadSprites();

    return true;
}

bool Game::OnUserUpdate(double deltaTime)
{
    m_raycaster.resetDepthBuffer();

    m_raycaster.renderFloor(&m_camera, 0.1);
    m_raycaster.renderCeiling(&m_camera, 0.1);
    m_raycaster.raycastRender(&m_camera, 0.01);

    m_sprites_rendered = 0;
    // loop through visited tiles
    for(auto tile : m_raycaster.getVisited())
    {
        // unpack x and y coordinate of visited tile
        int x = (int)utility::unpack_u<0>(tile);
        int y = (int)utility::unpack_u<1>(tile);
        for(const auto& sprite: m_sprites)
        {
            // if the sprite is in one of the visited tiles
            if((static_cast<int>(sprite.x) == x) && (static_cast<int>(sprite.y) == y))
            {
                // draw the sprite
                draw3DSprite("pixelbuffer", &m_camera, m_width, m_height, 1.0, sprite);
            }
        }
    }

    m_raycaster.renderBuffer();

    if(m_show_map)
    {
        m_raycaster.drawMinimap("pixelBuffer", m_camera, 2);
    }

    handle_input(deltaTime);

    if(m_show_fps)
    {
        write_text("PixelWolf - " + std::to_string( get_fps()) + " FPS");
    }
    else
    {
        write_text("PixelWolf");
    }

    return true;
}

bool Game::OnUserRender()
{
    SDL_UpdateTexture(m_render_texture.get(), NULL, m_pixelator.get()->getPixels(), sizeof(SDL_Color) * m_width);
    return true;
}

bool Game::OnUserDestroy()
{
    return true;
}

bool Game::initSpriteTexture(Texture* texture, const std::string& path, int tile_width, int tile_height, int num_tiles)
{
    texture->tile_width = tile_width;
    texture->tile_height = tile_height;
    texture->tile_count = num_tiles;

    int32_t mPixWidth;
    int32_t mPixHeight;
    uint8_t* rgbaData = stbi_load(path.c_str(), &mPixWidth, &mPixHeight, NULL, 0);
    if (!rgbaData)
    {
        SPDLOG_ERROR("Could not load texture '{}'", path);
        return false;
    }

    uint32_t newPix = 0;
    for (uint32_t p = 0; p < tile_width * tile_height * num_tiles; p++)
    {
        // Get each component
        for (uint8_t comp = 0; comp < 4; comp++)
        {
            newPix |= ((uint32_t)(rgbaData[p*4+comp]) << (8 * (3-comp)));
        }
        texture->pixels.push_back(newPix);
        newPix = 0;
    }
    stbi_image_free(rgbaData);
    return true;
}

void Game::initSprite(const std::string& type_name, Sprite* newSprite, Texture texture, double scaleFactor, double alphaNum, double x, double y, double h)
{
    newSprite->type_name = type_name;
    newSprite->texture = texture;
    newSprite->scaleFactor = scaleFactor;
    newSprite->alphaNum = alphaNum;
    newSprite->x = x;
    newSprite->y = y;
    newSprite->h = h;
    newSprite->frameNum = 0;
}

void Game::draw3DSprite(const std::string& buffer, Camera* camera, uint32_t width, uint32_t height, double resolution, Sprite sprite)
{
    double scaleFactor = (double)width / (double)height * 2.4;
    // Generate screenspace angle mapping constant
    const double angleMapConstant = (double)(width) / (2*tan(camera->fov/2));
    // Render sprite to buffer
    double spriteAngle = atan2(sprite.y - camera->y, sprite.x - camera->x);
    double screenAngle = spriteAngle - camera->angle;
    //printf("Sprite %d screen angle: %f\n", s, screenAngle);
    double spriteDist = cos(screenAngle) * (sqrt((camera->x - sprite.x)*(camera->x - sprite.x) + (camera->y - sprite.y)*(camera->y - sprite.y))/scaleFactor);

    // Depth check, can't be on or behind camera
    if (spriteDist > 0)
    {
        m_sprites_rendered++;
        // Compute column from screen angle
        int32_t centerX = (int32_t)floor(width / 2 + (int32_t)(angleMapConstant * tan(screenAngle)));
        // Get width and height
        int32_t screenHeight;
        int32_t screenWidth;
        if (sprite.texture.tile_height >= sprite.texture.tile_width)
        {
            screenHeight = (int32_t)((double)height / (spriteDist * 5) * sprite.scaleFactor);
            screenWidth = (int32_t)((double)screenHeight * ((double)sprite.texture.tile_width / (double)sprite.texture.tile_height));
        }
        else
        {
            screenWidth = (int32_t)ceil((double)height / (spriteDist * 5) * sprite.scaleFactor);
            screenHeight = (int32_t)ceil((double)screenWidth * ((double)sprite.texture.tile_height / (double)sprite.texture.tile_width));
        }
        
        int32_t spriteHeight = (int32_t)((sprite.h - camera->h) * height / (spriteDist * 5)); // I dunno why it's 40
        int32_t startX = centerX - screenWidth / 2;
        int32_t endX = startX + screenWidth;
        int32_t startY = (int32_t)ceil((height / 2) - ((double)screenHeight / 2) - spriteHeight);
        // Write to buffer if in fulcrum
        if (startX <= (int32_t)width && endX >= 0)
        {
            // Iterate through screen columns
            uint32_t spriteColumn = 0;
            uint32_t texCoord;
            for (int32_t i = startX; i < endX; i++)
            {
                if (i >= 0 && i < width)
                {
                    double colorGrad;
                    double fogConstant = 1.5/5;
                    if (spriteDist < (camera->dist*fogConstant))
                    {
                        colorGrad = (spriteDist) / (camera->dist*fogConstant);
                    }
                    else
                    {
                        colorGrad = 1.0;
                    }
                    SDL_Color fog_color = {50,20,50,255};
                    texCoord = (uint32_t)floor(((double)spriteColumn / (double)screenWidth) * sprite.texture.tile_width);
                    m_raycaster.drawTextureColumnEx(
                        i, startY,
                        screenHeight, spriteDist,
                        sprite.texture,
                        sprite.frameNum, sprite.alphaNum,
                        texCoord, colorGrad,
                        fog_color
                    );
                }
                spriteColumn++;
            }
        }
    }
}

void Game::loadSprites()
{
    utility::Map* map = m_map.get();

    std::vector<utility::MapItem> map_statics = map->statics();
    std::vector<utility::MapItem> map_pickups = map->pickups();

    for(const auto& static_item: map_statics)
    {
        addStatic(static_item.type, static_item.map_x, static_item.map_y);
    }
    for(const auto& pickup: map_pickups)
    {
        addPickup(pickup.type, pickup.map_x, pickup.map_y);
    }
}

void Game::addStatic(const std::string& type, int x, int y)
{
    Texture sprite_texture;
    Sprite sprite;
    std::string sprite_texture_path = "";

    // { "id": "Planter", "tileId": null, "__tileSrcRect": null },
    if(type == "Planter")
    {
        sprite_texture_path = "assets/sprites/static/planter.png";
    }
    // { "id": "Table", "tileId": null, "__tileSrcRect": null },
    else if(type == "Table")
    {
        sprite_texture_path = "assets/sprites/static/table.png";
    }
    // { "id": "Table_Small", "tileId": null, "__tileSrcRect": null },
    else if(type == "Table_Small")
    {
        sprite_texture_path = "assets/sprites/static/table_small.png";
    }
    // { "id": "Bowl", "tileId": null, "__tileSrcRect": null },
    else if(type == "Bowl")
    {
        sprite_texture_path = "assets/sprites/static/claypot.png";
    }
    // { "id": "Urn", "tileId": null, "__tileSrcRect": null },
    else if(type == "Urn")
    {
        sprite_texture_path = "assets/sprites/static/urn.png";
    }
    // { "id": "Body", "tileId": null, "__tileSrcRect": null },
    else if(type == "Body")
    {
        sprite_texture_path = "assets/sprites/static/body.png";
    }
    // { "id": "Wooden_Barrel", "tileId": null, "__tileSrcRect": null },
    else if(type == "Wooden_Barrel")
    {
        sprite_texture_path = "assets/sprites/static/barrel_wood.png";
    }
    // { "id": "Green_Barrel", "tileId": null, "__tileSrcRect": null },
    else if(type == "Green_Barrel")
    {
        sprite_texture_path = "assets/sprites/static/barrel_green.png";
    }
    // { "id": "Cloth", "tileId": null, "__tileSrcRect": null },
    else if(type == "Cloth")
    {
        sprite_texture_path = "assets/sprites/static/cloth.png";
    }
    // { "id": "Flag", "tileId": null, "__tileSrcRect": null },
    else if(type == "Flag")
    {
        sprite_texture_path = "assets/sprites/static/flag.png";
    }
    // { "id": "Ceil_Gold", "tileId": null, "__tileSrcRect": null },
    else if(type == "Ceil_Gold")
    {
        sprite_texture_path = "assets/sprites/static/ceil_gold.png";
    }
    // { "id": "Ceil_Green", "tileId": null, "__tileSrcRect": null },
    else if(type == "Ceil_Green")
    {
        sprite_texture_path = "assets/sprites/static/ceil_green.png";
    }
    // { "id": "Stove", "tileId": null, "__tileSrcRect": null },
    else if(type == "Stove")
    {
        sprite_texture_path = "assets/sprites/static/stove.png";
    }
    // { "id": "Bones", "tileId": null, "__tileSrcRect": null },
    else if(type == "Bones")
    {
        sprite_texture_path = "assets/sprites/static/skulls.png";
    }
    // { "id": "Skel_Remain", "tileId": null, "__tileSrcRect": null },
    else if(type == "Skel_Remain")
    {
        sprite_texture_path = "assets/sprites/static/skeletal_remains.png";
    }
    // { "id": "Well_Dry", "tileId": null, "__tileSrcRect": null },
    else if(type == "Well_Dry")
    {
        sprite_texture_path = "assets/sprites/static/well.png";
    }
    // { "id": "Well_Water", "tileId": null, "__tileSrcRect": null },
    else if(type == "Well_Water")
    {
        sprite_texture_path = "assets/sprites/static/well_water.png";
    }
    // { "id": "Lamp", "tileId": null, "__tileSrcRect": null },
    else if(type == "Lamp")
    {
        sprite_texture_path = "assets/sprites/static/lamp.png";
    }
    // { "id": "Tree", "tileId": null, "__tileSrcRect": null }
    else if(type == "Tree")
    {
        sprite_texture_path = "assets/sprites/static/tree.png";
    }
    // { "id": "Sink", "tileId": null, "__tileSrcRect": null },
    else if(type == "Sink")
    {
        sprite_texture_path = "assets/sprites/static/sink.png";
    }
    // { "id": "Skel_Hang", "tileId": null, "__tileSrcRect": null },
    else if(type == "Skel_Hang")
    {
        sprite_texture_path = "assets/sprites/static/skeleton_hanging.png";
    }
    // { "id": "Pots_Pans", "tileId": null, "__tileSrcRect": null },
    else if(type == "Pots_Pans")
    {
        sprite_texture_path = "assets/sprites/static/kitchen_ware.png";
    }
    // { "id": "Bloody_Bones", "tileId": null, "__tileSrcRect": null },
    else if(type == "Bloody_Bones")
    {
        sprite_texture_path = "assets/sprites/static/blood_bones.png";
    }
    // { "id": "Armor", "tileId": null, "__tileSrcRect": null }
    else if(type == "Armor")
    {
        sprite_texture_path = "assets/sprites/static/armor.png";
    }
    // { "id": "Pillar", "tileId": null, "__tileSrcRect": null }
    else if(type == "Pillar")
    {
        sprite_texture_path = "assets/sprites/static/pillar.png";
    }
    else
    {
        sprite_texture_path = "assets/sprites/unknown.png";
    }


    if (!initSpriteTexture(&sprite_texture, sprite_texture_path, 64, 64, 1))
    {
        return;
    }
    initSprite(type, &sprite, sprite_texture, 1.0, 1.0, x + 0.5, y + 0.5, 0);
    m_sprites.push_back(sprite);
}

void Game::addPickup(const std::string& type, int x, int y)
{
    Texture sprite_texture;
    Sprite sprite;
    std::string sprite_texture_path = "";

    // { "id": "Dogfood", "tileId": null, "__tileSrcRect": null },
    if(type == "Dogfood")
    {
        sprite_texture_path = "assets/sprites/items/dogfood.png";
    }
    // { "id": "Meal", "tileId": null, "__tileSrcRect": null },
    else if(type == "Meal")
    {
        sprite_texture_path = "assets/sprites/items/food.png";
    }
    // { "id": "Health", "tileId": null, "__tileSrcRect": null },
    else if(type == "Health")
    {
        sprite_texture_path = "assets/sprites/items/health.png";
    }
    // { "id": "Ammo", "tileId": null, "__tileSrcRect": null },
    else if(type == "Ammo")
    {
        sprite_texture_path = "assets/sprites/items/ammo.png";
    }
    // { "id": "Machinegun", "tileId": null, "__tileSrcRect": null },
    else if(type == "Machinegun")
    {
        sprite_texture_path = "assets/sprites/items/machinegun.png";
    }
    // { "id": "Chaingun", "tileId": null, "__tileSrcRect": null },
    else if(type == "Chaingun")
    {
        sprite_texture_path = "assets/sprites/items/chaingun.png";
    }
    // { "id": "Gold_Cross", "tileId": null, "__tileSrcRect": null },
    else if(type == "Gold_Cross")
    {
        sprite_texture_path = "assets/sprites/items/gold_cross.png";
    }
    // { "id": "Gold_Goblet", "tileId": null, "__tileSrcRect": null },
    else if(type == "Gold_Goblet")
    {
        sprite_texture_path = "assets/sprites/items/gold_goblet.png";
    }
    // { "id": "Gold_Casket", "tileId": null, "__tileSrcRect": null },
    else if(type == "Gold_Casket")
    {
        sprite_texture_path = "assets/sprites/items/gold_casket.png";
    }
    // { "id": "Gold_Crown", "tileId": null, "__tileSrcRect": null }
    else if(type == "Gold_Crown")
    {
        sprite_texture_path = "assets/sprites/items/gold_crown.png";
    }
    else if(type == "Gold_Key")
    {
        sprite_texture_path = "assets/sprites/items/key_gold.png";
    }
    else if(type == "Silver_Key")
    {
        sprite_texture_path = "assets/sprites/items/key_silver.png";
    }
    else
    {
        sprite_texture_path = "assets/sprites/unknown.png";
    }

    if (!initSpriteTexture(&sprite_texture, sprite_texture_path, 64, 64, 1))
    {
        return;
    }
    initSprite(type, &sprite, sprite_texture, 1.0, 1.0, x + 0.5, y + 0.5, 0);
    m_sprites.push_back(sprite);
}
