#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct renderbuffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::renderbuffer>(legion::rendering::renderbuffer& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::renderbuffer>(const legion::rendering::renderbuffer& obj);
}
