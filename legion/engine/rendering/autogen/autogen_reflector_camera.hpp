#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct camera;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::camera>(legion::rendering::camera& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::camera>(const legion::rendering::camera& obj);
}
