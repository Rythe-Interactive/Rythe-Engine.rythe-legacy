#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::attribute>(const legion::rendering::attribute& obj);
}
