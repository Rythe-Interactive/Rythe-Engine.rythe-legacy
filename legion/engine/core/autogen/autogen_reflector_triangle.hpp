#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::math
{
    struct triangle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::math::triangle>(legion::core::math::triangle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::math::triangle>(const legion::core::math::triangle& obj);
}
