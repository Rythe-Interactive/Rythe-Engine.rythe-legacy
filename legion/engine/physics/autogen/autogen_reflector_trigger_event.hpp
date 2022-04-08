#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct trigger_event;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::trigger_event>(legion::physics::trigger_event& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::trigger_event>(const legion::physics::trigger_event& obj);
}
