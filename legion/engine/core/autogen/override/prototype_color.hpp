#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::math
{
    struct color;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::math::color>(const legion::core::math::color& obj);
}
