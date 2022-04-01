#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct scale;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::scale>(legion::core::scale& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::scale>(const legion::core::scale& obj);
}
