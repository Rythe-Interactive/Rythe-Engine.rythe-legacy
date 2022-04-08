#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct ring_sync_lock;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::ring_sync_lock>(const legion::core::async::ring_sync_lock& obj);
}
