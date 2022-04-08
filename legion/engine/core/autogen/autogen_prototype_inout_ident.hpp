#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute
{
    struct inout_ident;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::inout_ident>(const legion::core::compute::inout_ident& obj);
}
