#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct texture;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::texture>(const legion::rendering::texture& obj);
}
