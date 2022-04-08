#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct async_operation_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::async_operation_base>(const legion::core::async::async_operation_base& obj);
}
