#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct shader_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::shader_handle>(legion::rendering::shader_handle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::shader_handle>(const legion::rendering::shader_handle& obj);
}
