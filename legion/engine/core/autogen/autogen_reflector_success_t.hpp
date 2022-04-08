#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::common
{
    struct success_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::common::success_t>(legion::core::common::success_t& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::common::success_t>(const legion::core::common::success_t& obj);
}
