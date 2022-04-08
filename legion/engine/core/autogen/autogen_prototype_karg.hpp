#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::compute
{
    struct karg;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::compute::karg>(const legion::core::compute::karg& obj);
}
