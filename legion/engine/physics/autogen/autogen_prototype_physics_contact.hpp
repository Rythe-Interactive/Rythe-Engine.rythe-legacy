#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct physics_contact;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::physics_contact>(const legion::physics::physics_contact& obj);
}
