#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::serialization
{
    struct json;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::serialization::json>(legion::core::serialization::json& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::serialization::json>(const legion::core::serialization::json& obj);
}
