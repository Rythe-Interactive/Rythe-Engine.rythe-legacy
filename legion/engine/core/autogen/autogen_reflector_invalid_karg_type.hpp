#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::compute
{
    struct invalid_karg_type;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::compute::invalid_karg_type>(legion::core::compute::invalid_karg_type& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::compute::invalid_karg_type>(const legion::core::compute::invalid_karg_type& obj);
}
