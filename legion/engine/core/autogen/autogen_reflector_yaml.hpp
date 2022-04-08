#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::serialization
{
    struct yaml;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::serialization::yaml>(legion::core::serialization::yaml& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::serialization::yaml>(const legion::core::serialization::yaml& obj);
}
