#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct screen_quad;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::screen_quad>(legion::rendering::screen_quad& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::screen_quad>(const legion::rendering::screen_quad& obj);
}
