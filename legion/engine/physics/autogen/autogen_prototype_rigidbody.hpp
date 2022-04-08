#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct rigidbody;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::rigidbody>(const legion::physics::rigidbody& obj);
}
