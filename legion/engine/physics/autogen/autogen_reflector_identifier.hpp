#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct identifier;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::identifier>(legion::physics::identifier& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::identifier>(const legion::physics::identifier& obj);
}
