#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::math
{
    struct color;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::math::color>(legion::core::math::color& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::math::color>(const legion::core::math::color& obj);
}
