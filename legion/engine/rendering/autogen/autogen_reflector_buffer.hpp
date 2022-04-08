#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct buffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::buffer>(legion::rendering::buffer& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::buffer>(const legion::rendering::buffer& obj);
}
