#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::serialization
{
    struct bson;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::serialization::bson>(legion::core::serialization::bson& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::serialization::bson>(const legion::core::serialization::bson& obj);
}
