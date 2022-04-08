#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct material_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::material_handle>(legion::rendering::material_handle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::material_handle>(const legion::rendering::material_handle& obj);
}
