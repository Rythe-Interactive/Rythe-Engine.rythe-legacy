#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct skybox_renderer;
}
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::skybox_renderer>(legion::rendering::skybox_renderer& obj);
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::skybox_renderer>(const legion::rendering::skybox_renderer& obj);
}
