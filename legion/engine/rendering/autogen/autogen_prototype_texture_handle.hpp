#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct texture_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::texture_handle>(const legion::rendering::texture_handle& obj);
}
