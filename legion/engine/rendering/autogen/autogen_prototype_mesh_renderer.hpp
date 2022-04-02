#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct mesh_renderer;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::mesh_renderer>(const legion::rendering::mesh_renderer& obj);
}
