#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct model;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::model>(legion::rendering::model& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::model>(const legion::rendering::model& obj);
}
