#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct texture;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::texture>(legion::rendering::texture& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::texture>(const legion::rendering::texture& obj);
}
