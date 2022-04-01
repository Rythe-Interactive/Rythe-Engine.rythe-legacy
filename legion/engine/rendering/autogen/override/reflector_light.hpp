#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct light;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::light>(legion::rendering::light& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::light>(const legion::rendering::light& obj);
}
