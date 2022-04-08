#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct async_runnable_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::async_runnable_base>(const legion::core::async::async_runnable_base& obj);
}
