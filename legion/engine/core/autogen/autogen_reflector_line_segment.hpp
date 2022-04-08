#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::math
{
    struct line_segment;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::math::line_segment>(legion::core::math::line_segment& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::math::line_segment>(const legion::core::math::line_segment& obj);
}
