#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct physicsComponent;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::physicsComponent>(legion::physics::physicsComponent& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::physicsComponent>(const legion::physics::physicsComponent& obj);
}
