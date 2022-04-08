#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct range_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::range_attribute>(legion::core::range_attribute& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::range_attribute>(const legion::core::range_attribute& obj);
}
