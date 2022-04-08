#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct shader;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::shader>(const legion::rendering::shader& obj);
}
