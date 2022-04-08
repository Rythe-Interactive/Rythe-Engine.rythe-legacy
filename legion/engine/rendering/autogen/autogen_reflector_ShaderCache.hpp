#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct ShaderCache;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::ShaderCache>(legion::rendering::ShaderCache& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::ShaderCache>(const legion::rendering::ShaderCache& obj);
}
