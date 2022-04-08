#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct model;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::model>(const legion::rendering::model& obj);
}
