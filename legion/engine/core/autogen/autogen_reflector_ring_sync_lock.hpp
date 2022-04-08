#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::async
{
    struct ring_sync_lock;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::async::ring_sync_lock>(legion::core::async::ring_sync_lock& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::async::ring_sync_lock>(const legion::core::async::ring_sync_lock& obj);
}
