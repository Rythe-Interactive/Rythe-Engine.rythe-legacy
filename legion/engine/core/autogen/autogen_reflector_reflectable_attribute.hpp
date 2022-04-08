#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct reflectable_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::reflectable_attribute>(legion::core::reflectable_attribute& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::reflectable_attribute>(const legion::core::reflectable_attribute& obj);
}
