#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct material_parameter_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::material_parameter_base>(legion::rendering::material_parameter_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::material_parameter_base>(const legion::rendering::material_parameter_base& obj);
}
