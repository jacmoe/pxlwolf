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
#   (c) 2020 - 2025 Jacob Moena
#
#   MIT License
#*/
#include "main/Level.hpp"
#include "components/components.hpp"
#include "main/Entity.hpp"

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

void Level::destroyEntity(const Entity entity)
{
    m_registry.destroy(entity);
}

bool check_key(EntityMap m, const std::string& key)
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

bool check_key(EntityTextureMap m, const EntityType key)
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

EntityType Level::getEntityType(const std::string& type)
{
    if(!check_key(m_entity_map, type))
    {
        return m_entity_map["Unknown"];
    }
    return m_entity_map[type];
}

std::string Level::getEntityTexture(const EntityType type)
{
    if(!check_key(m_entitytexture_map, type))
    {
        return m_entitytexture_map[EntityType::none];
    }
    return m_entitytexture_map[type];
}

EntityCategory Level::getEntityCategory(const EntityType type)
{
    EntityCategory cat;
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
        case EntityType::cage:
        case EntityType::cage_skel:
        case EntityType::pots_pans:
        case EntityType::bloody_bones:
        case EntityType::armor:
        case EntityType::pillar:
        case EntityType::blood:
        case EntityType::bunk:
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
            cat = EntityCategory::key;
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
