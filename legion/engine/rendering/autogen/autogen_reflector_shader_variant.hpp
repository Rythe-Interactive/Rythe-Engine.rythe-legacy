#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct shader_variant;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::shader_variant>(legion::rendering::shader_variant& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::shader_variant>(const legion::rendering::shader_variant& obj);
}
