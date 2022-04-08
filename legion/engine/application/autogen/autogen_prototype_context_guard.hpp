#pragma once
#include <core/types/prototype.hpp>
namespace legion::application
{
    struct context_guard;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::application::context_guard>(const legion::application::context_guard& obj);
}
