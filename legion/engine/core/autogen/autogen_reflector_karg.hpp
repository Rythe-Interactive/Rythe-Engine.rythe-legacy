#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::compute
{
    struct karg;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::compute::karg>(legion::core::compute::karg& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::compute::karg>(const legion::core::compute::karg& obj);
}
