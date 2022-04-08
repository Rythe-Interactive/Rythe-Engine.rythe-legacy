#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct material;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::material>(legion::rendering::material& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::material>(const legion::rendering::material& obj);
}
