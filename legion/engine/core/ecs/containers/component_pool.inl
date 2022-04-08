#include <core/ecs/registry.hpp>
#include <core/ecs/containers/component_pool.hpp>
#pragma once

namespace legion::core::ecs
{
    template<typename ComponentType>
    inline L_ALWAYS_INLINE void component_pool<ComponentType>::reserve(size_type size)
    {
        m_components.reserve(size);
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE void component_pool<ComponentType>::clear()
    {
        m_components.clear();
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<void> component_pool<ComponentType>::create_component(entity target)
    {
        auto& ret = m_components.try_emplace(target).first.value();

        if constexpr (has_static_init_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::init(ret, target);
        }
        else if constexpr (has_static_init_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::init(ret);
        }

        events::EventBus::raiseEvent<events::component_creation<ComponentType>>(target);

        return { &ret };
    }

    template<typename ComponentType>
    template<typename... Args>
    inline L_ALWAYS_INLINE ComponentType& component_pool<ComponentType>::create_component(entity target, Args&& ...args)
    {
        auto& ret = m_components.try_emplace(target, std::forward<Args>(args)...).first.value();

        if constexpr (has_static_init_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::init(ret, target);
        }
        else if constexpr (has_static_init_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::init(ret);
        }

        events::EventBus::raiseEvent<events::component_creation<ComponentType>>(target);

        return ret;
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<void> component_pool<ComponentType>::create_component(entity target, const prototype& src)
    {
        ComponentType* ret;
        if constexpr (::std::is_constructible_v<ComponentType, const prototype&>)
            ret = &m_components.try_emplace(target, src).first.value();
        else
            ret = &m_components.try_emplace(target).first.value();

        if constexpr (has_static_init_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::init(*ret, target);
        }
        else if constexpr (has_static_init_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::init(*ret);
        }

        events::EventBus::raiseEvent<events::component_creation<ComponentType>>(target);

        return { ret };
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<void> component_pool<ComponentType>::create_component(entity target, prototype&& src)
    {
        ComponentType* ret;
        if constexpr (::std::is_constructible_v<ComponentType, prototype&&> || ::std::is_constructible_v<ComponentType, const prototype&>)
            ret = &m_components.try_emplace(target, std::move(src)).first.value();
        else
            ret = &m_components.try_emplace(target).first.value();

        if constexpr (has_static_init_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::init(*ret, target);
        }
        else if constexpr (has_static_init_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::init(*ret);
        }

        events::EventBus::raiseEvent<events::component_creation<ComponentType>>(target);

        return { ret };
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<void> component_pool<ComponentType>::create_component(entity target, pointer<const void> src)
    {
        ComponentType* ret;
        if constexpr (::std::is_constructible_v<ComponentType, const ComponentType&>)
            ret = &m_components.try_emplace(target, *reinterpret_cast<const ComponentType*>(src.ptr)).first.value();
        else
            ret = &m_components.try_emplace(target).first.value();

        if constexpr (has_static_init_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::init(*ret, target);
        }
        else if constexpr (has_static_init_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::init(*ret);
        }

        events::EventBus::raiseEvent<events::component_creation<ComponentType>>(target);

        return { ret };
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE bool component_pool<ComponentType>::contains(entity target) const
    {
        return m_components.contains(target);
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE void component_pool<ComponentType>::destroy_component(entity target)
    {
        events::EventBus::raiseEvent<events::component_destruction<ComponentType>>(target);

        if constexpr (has_static_destroy_v<ComponentType, void(ComponentType&, entity)>)
        {
            ComponentType::destroy(m_components.at(target->id), target);
        }
        else if constexpr (has_static_destroy_v<ComponentType, void(ComponentType&)>)
        {
            ComponentType::destroy(m_components.at(target->id));
        }

        m_components.erase(target->id);
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<void> component_pool<ComponentType>::get_component(entity target)
    {
        return { &(m_components.at(target)) };
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE pointer<const void> component_pool<ComponentType>::get_component(entity target) const
    {
        return { &(m_components.at(target)) };
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE reflector component_pool<ComponentType>::get_component_reflector(entity target)
    {
        return make_reflector(m_components.at(target));
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE const reflector component_pool<ComponentType>::get_component_reflector(entity target) const
    {
        return make_reflector(m_components.at(target));
    }

    template<typename ComponentType>
    inline L_ALWAYS_INLINE void component_pool<ComponentType>::fill_container(component_container<ComponentType>& container, entity_set& entities)
    {
        container.reserve(entities.size());

        for (auto ent : entities)
            container.emplace_back(m_components.at(ent));
    }

}
