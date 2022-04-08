#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct HalfEdgeEdge;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::HalfEdgeEdge>(const legion::physics::HalfEdgeEdge& obj);
}
