#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::compute
{
    struct out_ident;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::compute::out_ident>(legion::core::compute::out_ident& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::compute::out_ident>(const legion::core::compute::out_ident& obj);
}
