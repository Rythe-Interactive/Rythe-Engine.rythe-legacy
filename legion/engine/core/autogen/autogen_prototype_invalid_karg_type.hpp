#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute
{
    struct invalid_karg_type;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::invalid_karg_type>(const legion::core::compute::invalid_karg_type& obj);
}
