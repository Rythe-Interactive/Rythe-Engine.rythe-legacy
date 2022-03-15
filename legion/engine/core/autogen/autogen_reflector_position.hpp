#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct position;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::position>(legion::core::position& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::position>(const legion::core::position& obj);
}
