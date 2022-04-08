#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::time
{
    struct main_clock_time;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::time::main_clock_time>(legion::core::time::main_clock_time& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::time::main_clock_time>(const legion::core::time::main_clock_time& obj);
}
