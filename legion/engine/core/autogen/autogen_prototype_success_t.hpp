#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::common
{
    struct success_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::common::success_t>(const legion::core::common::success_t& obj);
}
