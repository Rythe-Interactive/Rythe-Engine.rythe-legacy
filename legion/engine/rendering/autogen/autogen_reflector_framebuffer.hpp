#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct framebuffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::framebuffer>(legion::rendering::framebuffer& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::framebuffer>(const legion::rendering::framebuffer& obj);
}
