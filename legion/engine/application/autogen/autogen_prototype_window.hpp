#pragma once
#include <core/types/prototype.hpp>
namespace legion::application
{
    struct window;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::application::window>(const legion::application::window& obj);
}
