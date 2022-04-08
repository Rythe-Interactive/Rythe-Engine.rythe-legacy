#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct VoronoiAABB;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::VoronoiAABB>(legion::physics::VoronoiAABB& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::VoronoiAABB>(const legion::physics::VoronoiAABB& obj);
}
