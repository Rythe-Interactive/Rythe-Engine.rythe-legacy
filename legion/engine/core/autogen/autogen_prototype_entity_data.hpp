#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct entity_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::entity_data>(const legion::core::ecs::entity_data& obj);
}
