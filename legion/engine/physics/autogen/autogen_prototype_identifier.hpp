#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct identifier;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::identifier>(const legion::physics::identifier& obj);
}
