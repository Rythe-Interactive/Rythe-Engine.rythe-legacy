#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::events
{
    struct event_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::events::event_base>(const legion::core::events::event_base& obj);
}
