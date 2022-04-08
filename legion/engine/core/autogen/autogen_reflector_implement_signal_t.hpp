#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::interfaces
{
    struct implement_signal_t;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::interfaces::implement_signal_t>(legion::core::interfaces::implement_signal_t& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::interfaces::implement_signal_t>(const legion::core::interfaces::implement_signal_t& obj);
}
