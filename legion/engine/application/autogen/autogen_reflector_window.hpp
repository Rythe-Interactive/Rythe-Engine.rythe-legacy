#pragma once
#include <core/types/reflector.hpp>
namespace legion::application
{
    struct window;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::application::window>(legion::application::window& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::application::window>(const legion::application::window& obj);
}
