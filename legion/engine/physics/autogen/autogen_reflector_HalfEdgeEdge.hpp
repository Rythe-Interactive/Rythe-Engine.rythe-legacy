#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct HalfEdgeEdge;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::HalfEdgeEdge>(legion::physics::HalfEdgeEdge& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::HalfEdgeEdge>(const legion::physics::HalfEdgeEdge& obj);
}
