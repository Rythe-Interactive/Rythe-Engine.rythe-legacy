#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::serialization
{
    struct serializer_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::serialization::serializer_base>(legion::core::serialization::serializer_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::serialization::serializer_base>(const legion::core::serialization::serializer_base& obj);
}
