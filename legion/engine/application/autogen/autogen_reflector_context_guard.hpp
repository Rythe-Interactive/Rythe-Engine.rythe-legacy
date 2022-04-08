#pragma once
#include <core/types/reflector.hpp>
namespace legion::application
{
    struct context_guard;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::application::context_guard>(legion::application::context_guard& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::application::context_guard>(const legion::application::context_guard& obj);
}
