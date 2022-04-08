#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct material_parameter_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::material_parameter_base>(const legion::rendering::material_parameter_base& obj);
}
