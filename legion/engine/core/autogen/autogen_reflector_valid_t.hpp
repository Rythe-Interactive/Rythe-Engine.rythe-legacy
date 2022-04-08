#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::common
{
    struct valid_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::common::valid_t>(legion::core::common::valid_t& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::common::valid_t>(const legion::core::common::valid_t& obj);
}
