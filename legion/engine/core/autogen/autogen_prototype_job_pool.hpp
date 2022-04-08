#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct job_pool;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::job_pool>(const legion::core::async::job_pool& obj);
}
