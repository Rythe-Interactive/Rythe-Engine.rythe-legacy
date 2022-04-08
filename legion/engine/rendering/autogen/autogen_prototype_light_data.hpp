#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering::detail
{
    struct light_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::detail::light_data>(const legion::rendering::detail::light_data& obj);
}
