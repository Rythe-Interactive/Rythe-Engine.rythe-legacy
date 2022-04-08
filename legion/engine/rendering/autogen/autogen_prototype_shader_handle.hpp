#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct shader_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::shader_handle>(const legion::rendering::shader_handle& obj);
}
