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
#include "Level.hpp"
#include "components.hpp"
#include "Entity.hpp"

Level::Level()
{
}

Level::~Level()
{
}

Entity Level::createEntity(const std::string& name)
{
    Entity entity = { m_registry.create(), this };
    auto& tag = entity.addComponent<components::TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;
    return entity;
}

void Level::destroyEntity(Entity entity)
{
    m_registry.destroy(entity);
}

bool check_key(EntityMap m, std::string key) 
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

bool check_key(EntityTextureMap m, EntityType key) 
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

EntityType Level::getEntityType(std::string type)
{
    if(!check_key(m_entity_map, type))
    {
        return m_entity_map["Unknown"];
    }
    return m_entity_map[type];
}

std::string Level::getEntityTexture(enum EntityType type)
{
    if(!check_key(m_entitytexture_map, type))
    {
        return m_entitytexture_map[EntityType::none];
    }
    return m_entitytexture_map[type];
}

enum EntityCategory Level::getEntityCategory(enum EntityType type)
{
    enum EntityCategory cat;
    switch(type)
    {
        case EntityType::none:
        case EntityType::planter:
        case EntityType::table:
        case EntityType::table_small:
        case EntityType::bowl:
        case EntityType::urn:
        case EntityType::body:
        case EntityType::barrel_wooden:
        case EntityType::barrel_green:
        case EntityType::cloth:
        case EntityType::flag:
        case EntityType::ceil_gold:
        case EntityType::ceil_green:
        case EntityType::stove:
        case EntityType::bones:
        case EntityType::skel_remain:
        case EntityType::well_dry:
        case EntityType::well_water:
        case EntityType::lamp:
        case EntityType::tree:
        case EntityType::sink:
        case EntityType::skel_hang:
        case EntityType::pots_pans:
        case EntityType::bloody_bones:
        case EntityType::armor:
        case EntityType::pillar:
            cat = EntityCategory::_static;
            break;
        // pickup
        case EntityType::dogfood:
        case EntityType::meal:
        case EntityType::health:
        case EntityType::ammo:
        case EntityType::machinegun:
        case EntityType::chaingun:
        case EntityType::gold_cross:
        case EntityType::gold_goblet:
        case EntityType::gold_casket:
        case EntityType::gold_crown:
            cat = EntityCategory::pickup;
            break;
        // keys
        case EntityType::gold_key:
        case EntityType::silver_key:
            return EntityCategory::key;
            break;
        // enemies
        case EntityType::guard:
        case EntityType::dog:
        case EntityType::officer:
        case EntityType::ss:
            cat = EntityCategory::enemy;
            break;
        default:
            cat = EntityCategory::none;
    };
    return cat;
}
