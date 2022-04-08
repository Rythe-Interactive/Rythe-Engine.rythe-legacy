#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct variant_submaterial;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::variant_submaterial>(const legion::rendering::variant_submaterial& obj);
}
