#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct vertexarray;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::vertexarray>(const legion::rendering::vertexarray& obj);
}
