#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct velocity;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::velocity>(legion::core::velocity& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::velocity>(const legion::core::velocity& obj);
}
