#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::async
{
    struct this_job;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::async::this_job>(legion::core::async::this_job& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::async::this_job>(const legion::core::async::this_job& obj);
}
