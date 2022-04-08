#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct physics_manifold_precursor;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::physics_manifold_precursor>(legion::physics::physics_manifold_precursor& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::physics_manifold_precursor>(const legion::physics::physics_manifold_precursor& obj);
}
