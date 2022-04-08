#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct EdgeLabel;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::EdgeLabel>(const legion::physics::EdgeLabel& obj);
}
