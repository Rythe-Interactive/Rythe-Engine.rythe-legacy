#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct no_reflect_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::no_reflect_attribute>(legion::core::no_reflect_attribute& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::no_reflect_attribute>(const legion::core::no_reflect_attribute& obj);
}
