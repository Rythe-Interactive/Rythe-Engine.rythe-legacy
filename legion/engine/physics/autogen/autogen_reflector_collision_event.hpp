#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct collision_event;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::collision_event>(legion::physics::collision_event& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::collision_event>(const legion::physics::collision_event& obj);
}
