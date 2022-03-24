#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct light;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::light>(const legion::rendering::light& obj);
}
