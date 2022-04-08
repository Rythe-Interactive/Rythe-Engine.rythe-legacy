#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct EdgeLabel;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::EdgeLabel>(legion::physics::EdgeLabel& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::EdgeLabel>(const legion::physics::EdgeLabel& obj);
}
