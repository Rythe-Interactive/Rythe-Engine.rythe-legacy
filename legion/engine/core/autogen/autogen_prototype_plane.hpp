#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::math
{
    struct plane;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::math::plane>(const legion::core::math::plane& obj);
}
