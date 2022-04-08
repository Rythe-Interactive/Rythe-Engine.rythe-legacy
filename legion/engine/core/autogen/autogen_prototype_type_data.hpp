#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::detail
{
    struct type_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::detail::type_data>(const legion::core::detail::type_data& obj);
}
