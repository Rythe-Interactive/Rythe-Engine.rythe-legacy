#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct framebuffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::framebuffer>(const legion::rendering::framebuffer& obj);
}
