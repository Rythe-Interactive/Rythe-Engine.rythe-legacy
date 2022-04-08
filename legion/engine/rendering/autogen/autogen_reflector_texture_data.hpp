#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct texture_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::texture_data>(legion::rendering::texture_data& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::texture_data>(const legion::rendering::texture_data& obj);
}
