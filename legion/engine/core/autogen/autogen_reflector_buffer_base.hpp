#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::compute::detail
{
    struct buffer_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::compute::detail::buffer_base>(legion::core::compute::detail::buffer_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::compute::detail::buffer_base>(const legion::core::compute::detail::buffer_base& obj);
}
