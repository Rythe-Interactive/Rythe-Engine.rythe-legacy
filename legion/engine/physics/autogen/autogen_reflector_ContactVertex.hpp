#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct ContactVertex;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::ContactVertex>(legion::physics::ContactVertex& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::ContactVertex>(const legion::physics::ContactVertex& obj);
}
