#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct vertexarray;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::vertexarray>(legion::rendering::vertexarray& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::vertexarray>(const legion::rendering::vertexarray& obj);
}
