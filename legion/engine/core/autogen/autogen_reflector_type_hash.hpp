#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct type_hash;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::type_hash>(legion::core::type_hash& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::type_hash>(const legion::core::type_hash& obj);
}
