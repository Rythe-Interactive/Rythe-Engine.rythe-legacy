#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct archetype_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::archetype_base>(const legion::core::ecs::archetype_base& obj);
}
