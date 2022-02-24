#pragma once
#include <physics/data/physics_component_data.hpp>

namespace legion::physics
{
    constexpr size_t invalid_physics_component = 0;

    struct physicsComponent
    {
        physicsComponentData physicsCompData;
        size_t physicsComponentID = invalid_physics_component;
    };
}
