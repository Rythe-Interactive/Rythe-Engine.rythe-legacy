#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct skybox_renderer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::skybox_renderer>(const legion::rendering::skybox_renderer& obj);
}
