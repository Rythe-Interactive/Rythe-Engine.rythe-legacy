#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct shader;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::shader>(legion::rendering::shader& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::shader>(const legion::rendering::shader& obj);
}
