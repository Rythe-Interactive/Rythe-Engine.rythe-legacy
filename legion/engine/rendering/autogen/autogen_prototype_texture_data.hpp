#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct texture_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::texture_data>(const legion::rendering::texture_data& obj);
}
