#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::math
{
    struct triangle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::math::triangle>(const legion::core::math::triangle& obj);
}
