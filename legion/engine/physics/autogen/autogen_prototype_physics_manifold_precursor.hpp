#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct physics_manifold_precursor;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::physics_manifold_precursor>(const legion::physics::physics_manifold_precursor& obj);
}
