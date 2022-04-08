#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct model_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::model_handle>(legion::rendering::model_handle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::model_handle>(const legion::rendering::model_handle& obj);
}
