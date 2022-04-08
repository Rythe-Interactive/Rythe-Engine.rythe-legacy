#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct ShaderCache;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::ShaderCache>(const legion::rendering::ShaderCache& obj);
}
