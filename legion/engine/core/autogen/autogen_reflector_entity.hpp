#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::ecs
{
    struct entity;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::ecs::entity>(legion::core::ecs::entity& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::ecs::entity>(const legion::core::ecs::entity& obj);
}
