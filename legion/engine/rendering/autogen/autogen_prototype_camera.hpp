#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct camera;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::camera>(const legion::rendering::camera& obj);
}
