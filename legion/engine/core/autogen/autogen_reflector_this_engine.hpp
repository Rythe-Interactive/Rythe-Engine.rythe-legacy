#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct this_engine;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::this_engine>(legion::core::this_engine& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::this_engine>(const legion::core::this_engine& obj);
}
