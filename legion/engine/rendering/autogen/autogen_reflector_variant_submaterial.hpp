#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct variant_submaterial;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::variant_submaterial>(legion::rendering::variant_submaterial& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::variant_submaterial>(const legion::rendering::variant_submaterial& obj);
}
