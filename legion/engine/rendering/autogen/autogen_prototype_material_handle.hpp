#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct material_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::material_handle>(const legion::rendering::material_handle& obj);
}
