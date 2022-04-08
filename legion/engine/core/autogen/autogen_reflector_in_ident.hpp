#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::compute
{
    struct in_ident;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::compute::in_ident>(legion::core::compute::in_ident& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::compute::in_ident>(const legion::core::compute::in_ident& obj);
}
