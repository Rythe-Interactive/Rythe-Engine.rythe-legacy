#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute
{
    struct in_ident;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::in_ident>(const legion::core::compute::in_ident& obj);
}
