#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::math
{
    struct line_segment;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::math::line_segment>(const legion::core::math::line_segment& obj);
}
