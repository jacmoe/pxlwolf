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
