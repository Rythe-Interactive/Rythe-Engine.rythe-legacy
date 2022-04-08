#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::async
{
    struct async_runnable_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::async::async_runnable_base>(legion::core::async::async_runnable_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::async::async_runnable_base>(const legion::core::async::async_runnable_base& obj);
}
