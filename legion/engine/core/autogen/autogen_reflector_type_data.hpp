#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::detail
{
    struct type_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::detail::type_data>(legion::core::detail::type_data& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::detail::type_data>(const legion::core::detail::type_data& obj);
}
