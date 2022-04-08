#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::async
{
    struct this_job;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::async::this_job>(const legion::core::async::this_job& obj);
}
