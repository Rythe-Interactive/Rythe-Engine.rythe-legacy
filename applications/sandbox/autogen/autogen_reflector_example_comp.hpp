#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct example_comp;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::example_comp>(legion::core::example_comp& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::example_comp>(const legion::core::example_comp& obj);
}
