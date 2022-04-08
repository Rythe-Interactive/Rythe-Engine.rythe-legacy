#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::time
{
    struct main_clock_time;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::time::main_clock_time>(const legion::core::time::main_clock_time& obj);
}
