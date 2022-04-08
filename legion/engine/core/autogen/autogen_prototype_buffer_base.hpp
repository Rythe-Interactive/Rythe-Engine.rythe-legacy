#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute::detail
{
    struct buffer_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::detail::buffer_base>(const legion::core::compute::detail::buffer_base& obj);
}
