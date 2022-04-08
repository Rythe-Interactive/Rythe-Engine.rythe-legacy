#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct attribute_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::attribute_base>(legion::core::attribute_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::attribute_base>(const legion::core::attribute_base& obj);
}
