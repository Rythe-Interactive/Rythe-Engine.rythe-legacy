#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct material_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::material_data>(const legion::core::material_data& obj);
}
