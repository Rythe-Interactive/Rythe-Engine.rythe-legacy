#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct renderbuffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::renderbuffer>(const legion::rendering::renderbuffer& obj);
}
