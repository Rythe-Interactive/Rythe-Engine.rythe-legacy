#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct mesh_renderer;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::mesh_renderer>(legion::rendering::mesh_renderer& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::mesh_renderer>(const legion::rendering::mesh_renderer& obj);
}
