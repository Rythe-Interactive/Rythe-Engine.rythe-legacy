#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::events
{
    struct event_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::events::event_base>(legion::core::events::event_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::events::event_base>(const legion::core::events::event_base& obj);
}
