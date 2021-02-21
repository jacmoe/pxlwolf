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

#include "entt.hpp"

#include "Map.hpp"

#include <unordered_map>

class Entity;

enum class EntityCategory
{
    none,
    _static,
    pickup,
    enemy,
    key
};

enum class EntityType
{
    none,
    // static
    planter,
    table,
    table_small,
    bowl,
    urn,
    body,
    barrel_wooden,
    barrel_green,
    cloth,
    flag,
    ceil_gold,
    ceil_green,
    stove,
    bones,
    skel_remain,
    well_dry,
    well_water,
    lamp,
    tree,
    sink,
    skel_hang,
    cage,
    cage_skel,
    pots_pans,
    bloody_bones,
    armor,
    pillar,
    blood,
    bunk,

    // pickup
    dogfood,
    meal,
    health,
    ammo,
    machinegun,
    chaingun,
    gold_cross,
    gold_goblet,
    gold_casket,
    gold_crown,
    gold_key,
    silver_key,

    // enemies
    guard,
    dog,
    officer,
    ss
};

using EntityMap = std::unordered_map<std::string, EntityType>;
using EntityTextureMap = std::unordered_map<EntityType, std::string>;

class Level
{
public:
    Level();
    ~Level();

    Entity createEntity(const std::string& name = std::string());
    void destroyEntity(Entity entity);

    EntityType getEntityType(std::string type);
    std::string getEntityTexture(enum EntityType type);
    enum EntityCategory getEntityCategory(enum EntityType type);

private:
	entt::registry m_registry;

    EntityMap m_entity_map
    {
        {"Unknown", EntityType::none},
        // _static
        {"Planter", EntityType::planter},
        {"Table", EntityType::table},
        {"Table_Small", EntityType::table_small},
        {"Bowl", EntityType::bowl},
        {"Urn", EntityType::urn},
        {"Body", EntityType::body},
        {"Wooden_Barrel", EntityType::barrel_wooden},
        {"Green_Barrel", EntityType::barrel_green},
        {"Cloth", EntityType::cloth},
        {"Flag", EntityType::flag},
        {"Ceil_Gold", EntityType::ceil_gold},
        {"Ceil_Green", EntityType::ceil_green},
        {"Stove", EntityType::stove},
        {"Bones", EntityType::bones},
        {"Skel_Remain", EntityType::skel_remain},
        {"Well_Dry", EntityType::well_dry},
        {"Well_Water", EntityType::well_water},
        {"Lamp", EntityType::lamp},
        {"Tree", EntityType::tree},
        {"Sink", EntityType::sink},
        {"Skel_Hang", EntityType::skel_hang},
        {"Cage", EntityType::cage},
        {"Cage_Skel", EntityType::cage_skel},
        {"Pots_Pans", EntityType::pots_pans},
        {"Bloody_Bones", EntityType::bloody_bones},
        {"Armor", EntityType::armor},
        {"Pillar", EntityType::pillar},
        {"Blood", EntityType::blood},
        {"Bunk", EntityType::bunk},
        // pickup
        {"Dogfood", EntityType::dogfood},
        {"Meal", EntityType::meal},
        {"Health", EntityType::health},
        {"Ammo", EntityType::ammo},
        {"Machinegun", EntityType::machinegun},
        {"Chaingun", EntityType::chaingun},
        {"Gold_Cross", EntityType::gold_cross},
        {"Gold_Goblet", EntityType::gold_goblet},
        {"Gold_Casket", EntityType::gold_casket},
        {"Gold_Crown", EntityType::gold_crown},
        // keys
        {"Gold", EntityType::gold_key},
        {"Silver", EntityType::silver_key},
        // enemies
        {"Guard", EntityType::guard},
        {"Dog", EntityType::dog},
        {"Officer", EntityType::officer},
        {"SS", EntityType::ss}
    };

EntityTextureMap m_entitytexture_map
{
        {EntityType::none,  "assets/sprites/unknown.png"},
        // _static
        {EntityType::planter, "assets/sprites/static/planter.png"},
        {EntityType::table, "assets/sprites/static/table.png"},
        {EntityType::table_small, "assets/sprites/static/table_small.png"},
        {EntityType::bowl, "assets/sprites/static/claypot.png"},
        {EntityType::urn, "assets/sprites/static/urn.png"},
        {EntityType::body, "assets/sprites/static/body.png"},
        {EntityType::barrel_wooden, "assets/sprites/static/barrel_wood.png"},
        {EntityType::barrel_green, "assets/sprites/static/barrel_green.png"},
        {EntityType::cloth, "assets/sprites/static/cloth.png"},
        {EntityType::flag, "assets/sprites/static/flag.png"},
        {EntityType::ceil_gold, "assets/sprites/static/ceil_gold.png"},
        {EntityType::ceil_green, "assets/sprites/static/ceil_green.png"},
        {EntityType::stove, "assets/sprites/static/stove.png"},
        {EntityType::bones, "assets/sprites/static/skulls.png"},
        {EntityType::skel_remain, "assets/sprites/static/skeletal_remains.png"},
        {EntityType::well_dry, "assets/sprites/static/well.png"},
        {EntityType::well_water, "assets/sprites/static/well_water.png"},
        {EntityType::lamp, "assets/sprites/static/lamp.png"},
        {EntityType::tree, "assets/sprites/static/tree.png"},
        {EntityType::sink, "assets/sprites/static/sink.png"},
        {EntityType::skel_hang, "assets/sprites/static/skeleton_hanging.png"},
        {EntityType::cage, "assets/sprites/static/cage_empty.png"},
        {EntityType::cage_skel, "assets/sprites/static/cage.png"},
        {EntityType::pots_pans, "assets/sprites/static/kitchen_ware.png"},
        {EntityType::bloody_bones, "assets/sprites/static/blood_bones.png"},
        {EntityType::armor, "assets/sprites/static/armor.png"},
        {EntityType::pillar, "assets/sprites/static/pillar.png"},
        {EntityType::blood, "assets/sprites/static/blood.png"},
        {EntityType::bunk, "assets/sprites/static/bunk.png"},
        // pickup
        {EntityType::dogfood, "assets/sprites/items/dogfood.png"},
        {EntityType::meal, "assets/sprites/items/food.png"},
        {EntityType::health, "assets/sprites/items/health.png"},
        {EntityType::ammo, "assets/sprites/items/ammo.png"},
        {EntityType::machinegun, "assets/sprites/items/machinegun.png"},
        {EntityType::chaingun, "assets/sprites/items/chaingun.png"},
        {EntityType::gold_cross, "assets/sprites/items/gold_cross.png"},
        {EntityType::gold_goblet, "assets/sprites/items/gold_goblet.png"},
        {EntityType::gold_casket, "assets/sprites/items/gold_casket.png"},
        {EntityType::gold_crown, "assets/sprites/items/gold_crown.png"},
        {EntityType::gold_key, "assets/sprites/items/key_gold.png"},
        {EntityType::silver_key, "assets/sprites/items/key_silver.png"},
        // enemies
        {EntityType::guard, "assets/sprites/enemies/guard.png"},
        {EntityType::dog, "assets/sprites/enemies/dog.png"},
        {EntityType::officer, "assets/sprites/enemies/officer.png"},
        {EntityType::ss, "assets/sprites/enemies/ss.png"}
};


    friend class Entity;
};
