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

#include <cassert>

#include "entt/entt.hpp"
#include "main/Level.hpp"

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Level* level);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        assert(!hasComponent<T>() && "Entity already has component!");
        T& component = m_level->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        //m_level->OnComponentAdded<T>(*this, component);
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        assert(hasComponent<T>() && "Entity does not have component!");
        return m_level->m_registry.get<T>(m_entity_handle);
    }

    template<typename T>
    bool hasComponent()
    {
        return m_level->m_registry.has<T>(m_entity_handle);
    }

    template<typename T>
    void removeComponent()
    {
        assert(hasComponent<T>() && "Entity does not have component!");
        m_level->m_registry.remove<T>(m_entity_handle);
    }

    operator bool() const { return m_entity_handle != entt::null; }
    operator entt::entity() const { return m_entity_handle; }
    operator uint32_t() const { return (uint32_t)m_entity_handle; }

    bool operator==(const Entity& other) const
    {
        return m_entity_handle == other.m_entity_handle && m_level == other.m_level;
    }

    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    entt::entity m_entity_handle { entt::null };
    Level* m_level = nullptr;
};
