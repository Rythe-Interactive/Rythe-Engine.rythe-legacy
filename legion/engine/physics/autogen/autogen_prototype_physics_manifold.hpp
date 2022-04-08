#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct physics_manifold;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::physics_manifold>(const legion::physics::physics_manifold& obj);
}
