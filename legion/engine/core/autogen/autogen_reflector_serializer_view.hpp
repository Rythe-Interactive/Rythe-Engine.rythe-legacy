#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::serialization
{
    struct serializer_view;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::serialization::serializer_view>(legion::core::serialization::serializer_view& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::serialization::serializer_view>(const legion::core::serialization::serializer_view& obj);
}
