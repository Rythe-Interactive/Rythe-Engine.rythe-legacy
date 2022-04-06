#pragma once
#include <physics/data/physics_component_data.hpp>

namespace legion::physics
{
    constexpr size_t invalid_physics_component = std::numeric_limits<size_type>::max();

    struct physics_component
    {
        PhysicsComponentData physicsCompData;
        size_t physicsComponentID = invalid_physics_component;
    };
}
