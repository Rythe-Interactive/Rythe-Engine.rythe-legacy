#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct rigidbody;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::rigidbody>(legion::physics::rigidbody& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::rigidbody>(const legion::physics::rigidbody& obj);
}
