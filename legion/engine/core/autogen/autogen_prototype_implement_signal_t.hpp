#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::interfaces
{
    struct implement_signal_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::interfaces::implement_signal_t>(const legion::core::interfaces::implement_signal_t& obj);
}
