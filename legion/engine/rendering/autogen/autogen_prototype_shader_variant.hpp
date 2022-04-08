#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct shader_variant;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::shader_variant>(const legion::rendering::shader_variant& obj);
}
