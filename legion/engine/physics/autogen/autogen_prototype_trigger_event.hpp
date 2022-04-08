#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct trigger_event;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::trigger_event>(const legion::physics::trigger_event& obj);
}
