#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct texture_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::texture_handle>(legion::rendering::texture_handle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::texture_handle>(const legion::rendering::texture_handle& obj);
}
