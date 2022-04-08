#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::common
{
    struct error_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::common::error_t>(const legion::core::common::error_t& obj);
}
