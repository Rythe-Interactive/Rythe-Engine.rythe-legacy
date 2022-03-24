#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct rotation;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::rotation>(legion::core::rotation& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::rotation>(const legion::core::rotation& obj);
}
