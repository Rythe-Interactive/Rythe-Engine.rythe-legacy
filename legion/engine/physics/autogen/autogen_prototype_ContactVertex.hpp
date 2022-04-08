#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct ContactVertex;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::ContactVertex>(const legion::physics::ContactVertex& obj);
}
