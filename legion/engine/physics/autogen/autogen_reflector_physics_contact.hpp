#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct physics_contact;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::physics_contact>(legion::physics::physics_contact& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::physics_contact>(const legion::physics::physics_contact& obj);
}
