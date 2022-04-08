#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::math
{
    struct plane;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::math::plane>(legion::core::math::plane& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::math::plane>(const legion::core::math::plane& obj);
}
