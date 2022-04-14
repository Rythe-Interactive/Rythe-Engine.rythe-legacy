#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    enum physics_component_flag : size_type
    {
        pc_add_first_box = 0ull,
        pc_add_first_sphere,
        pc_add_first_convex,
        pc_add_next_box,
        pc_add_next_sphere,
        pc_add_next_convex,
        pc_max //must always be the last enum
    };

    enum collider_modification_flag : size_type
    {
        cm_set_new_material = 0ull,
        cm_set_new_convex_mesh,
        cm_set_new_box_extents,
        cm_max //must always be the last enum
    };

    enum class collider_type
    {
        box,
        quickhull_convex,
        sphere,
        not_set //internal use only
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

    enum physics_enviroment_flag : size_type
    {
        pe_add_plane,
        pe_max
    };

    enum class physics_enviroment_object_type
    {
        plane,
        height_field,
        not_set
    };

    enum capsule_character_flag : size_type
    {
        cc_set_radius,
        cc_set_height,
        cc_teleport_to,
        cc_move_to,
        cc_set_gravity,
        cc_max
    };
}
