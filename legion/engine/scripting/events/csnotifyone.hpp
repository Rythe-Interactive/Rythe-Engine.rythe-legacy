#pragma once
#include <core/events/event.hpp>


namespace legion::scripting
{
    struct csnotifyone : core::events::event<csnotifyone>
    {
        int payload = 0;
    };
}
