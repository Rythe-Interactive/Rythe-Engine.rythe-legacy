#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct screen_quad;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::screen_quad>(const legion::rendering::screen_quad& obj);
}
