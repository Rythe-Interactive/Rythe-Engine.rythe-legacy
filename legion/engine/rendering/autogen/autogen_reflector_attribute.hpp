#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::attribute>(legion::rendering::attribute& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::attribute>(const legion::rendering::attribute& obj);
}
