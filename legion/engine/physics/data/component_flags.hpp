#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    enum physics_component_flag : size_type
    {
        pc_add_box = 0ull,
        pc_add_convex,
        pc_add_sphere,
        pc_add_capsule,
        pc_max
    };
}
