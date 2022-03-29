#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering::detail
{
    struct light_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::detail::light_data>(legion::rendering::detail::light_data& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::detail::light_data>(const legion::rendering::detail::light_data& obj);
}
