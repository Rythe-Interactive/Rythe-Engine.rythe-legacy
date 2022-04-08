#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::async
{
    struct job_pool;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::async::job_pool>(legion::core::async::job_pool& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::async::job_pool>(const legion::core::async::job_pool& obj);
}
