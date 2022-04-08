#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct physics_manifold;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::physics_manifold>(legion::physics::physics_manifold& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::physics_manifold>(const legion::physics::physics_manifold& obj);
}
