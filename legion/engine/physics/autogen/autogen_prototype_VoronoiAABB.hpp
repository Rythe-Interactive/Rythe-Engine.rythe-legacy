#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct VoronoiAABB;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::VoronoiAABB>(const legion::physics::VoronoiAABB& obj);
}
