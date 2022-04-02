#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct material_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::material_data>(legion::core::material_data& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::material_data>(const legion::core::material_data& obj);
}
