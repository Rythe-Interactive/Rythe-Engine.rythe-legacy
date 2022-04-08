#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct buffer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::buffer>(const legion::rendering::buffer& obj);
}
