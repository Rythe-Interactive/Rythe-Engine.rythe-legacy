#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    enum physics_component_flag : size_type
    {
        pc_add_first_box = 0ull,
        pc_add_next_box,
        pc_max
    };
}
