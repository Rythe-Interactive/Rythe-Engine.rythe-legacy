#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct entity;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::entity>(const legion::core::ecs::entity& obj);
}
