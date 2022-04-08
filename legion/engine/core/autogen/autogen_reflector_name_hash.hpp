#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct name_hash;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::name_hash>(legion::core::name_hash& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::name_hash>(const legion::core::name_hash& obj);
}
