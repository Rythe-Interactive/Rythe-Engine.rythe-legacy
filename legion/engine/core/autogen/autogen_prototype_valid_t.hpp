#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::common
{
    struct valid_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::common::valid_t>(const legion::core::common::valid_t& obj);
}
