#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct mesh;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::mesh>(legion::core::mesh& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::mesh>(const legion::core::mesh& obj);
}
