#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::ecs
{
    struct component_pool_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::ecs::component_pool_base>(legion::core::ecs::component_pool_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::ecs::component_pool_base>(const legion::core::ecs::component_pool_base& obj);
}
