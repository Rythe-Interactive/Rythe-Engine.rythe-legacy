#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct collision_event;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::collision_event>(const legion::physics::collision_event& obj);
}
