#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute
{
    struct out_ident;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::out_ident>(const legion::core::compute::out_ident& obj);
}
