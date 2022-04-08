#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::ecs
{
    struct entity_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::ecs::entity_data>(legion::core::ecs::entity_data& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::ecs::entity_data>(const legion::core::ecs::entity_data& obj);
}
