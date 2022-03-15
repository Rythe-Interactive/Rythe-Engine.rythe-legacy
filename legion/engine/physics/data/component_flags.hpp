#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    enum physics_component_flag : size_type
    {
        pc_add_first_box = 0ull,
        pc_add_first_sphere,
        pc_add_next_box,
        pc_add_next_sphere,
        pc_max
    };

    enum class collider_type
    {
        box,
        quickhull_convex,
        sphere,
        not_set
    };

    enum rigidbody_flag : size_type
    {
        rb_velocity,
        rb_angular_velocity,
        rb_inertia_tensor,
        rb_mass,
        rb_linear_drag,
        rb_angular_drag,
        rb_max //must always be the last enum
    };
}
